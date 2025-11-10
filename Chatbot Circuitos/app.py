# --- Imports ---
import base64
import os
import time
import io
from typing import Dict
from concurrent.futures import ThreadPoolExecutor

import streamlit as st
import openai
from pydantic import BaseModel
from PIL import Image
import google.generativeai as genai
from google.generativeai import types


# --- Configuração da Aplicação ---
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))


# --- Modelos de Configuração ---
class AssistantConfig(BaseModel):
    id: str
    name: str
    description: str


AVAILABLE_ASSISTANTS: Dict[str, AssistantConfig] = {
    "GPT_Solucionador": AssistantConfig(
        id="asst_6tRDhqUaB3njj1bnz7u4pY5k",
        name="GPT Solucionador (com histórico)",
        description="Resolve circuitos usando banco de provas antigas",
    ),
    "Gemini_Solucionador": AssistantConfig(
        id="gemini-2.5-pro",  # Modelo Gemini
        name="Gemini Solucionador",
        description="Análise independente de circuitos",
    ),
    "Consolidador": AssistantConfig(
        id="asst_H5BdjOw8RRv60QLDWHBVzXmu",
        name="Consolidador Expert",
        description="Compara e consolida soluções de circuitos",
    ),
}

DEFAULT_ASSISTANT = "Prova_Circuitos"


# --- Funções de Processamento ---
def process_with_gpt(
    prompt: str, image_data: str | None = None, thread_id: str | None = None
) -> dict:
    """Processa a mensagem com o GPT Assistant"""
    try:
        assistant_info = AVAILABLE_ASSISTANTS[st.session_state.assistant_key]

        if not thread_id:
            thread = client.beta.threads.create()
            thread_id = thread.id

        # Prepara o conteúdo da mensagem
        if image_data:
            api_message_content = [
                {"type": "text", "text": f"Analise esta imagem e responda: {prompt}"},
                {
                    "type": "image_url",
                    "image_url": {"url": image_data, "detail": "high"},
                },
            ]
        else:
            api_message_content = prompt

        # Adiciona mensagem à thread
        client.beta.threads.messages.create(
            thread_id=thread_id,
            role="user",
            content=api_message_content,
        )

        # Executa e aguarda conclusão
        run = client.beta.threads.runs.create_and_poll(
            thread_id=thread_id,
            assistant_id=assistant_info.id,
        )

        if run.status == "completed":
            messages = client.beta.threads.messages.list(thread_id=thread_id)
            response = messages.data[0].content[0].text.value
            return {"success": True, "response": response, "thread_id": thread_id}
        else:
            return {"success": False, "error": f"Run status: {run.status}"}

    except Exception as e:
        return {"success": False, "error": str(e)}


def process_with_gemini(prompt: str, image_data: str | None = None) -> dict:
    """Processa a mensagem com o Gemini"""
    try:
        gemini_client = genai.Client(
            api_key=st.secrets.get("GEMINI_API_KEY") or os.environ.get("GEMINI_API_KEY")
        )

        system_instruction = """
Você é um engenheiro elétrico especializado em análise de circuitos, conhecido por sua abordagem metódica e rigorosa.

OBJETIVO:
Resolver problemas de circuitos elétricos apresentados em imagens com máxima precisão e clareza.

ABORDAGEM:
1. OBSERVAÇÃO: Analise cuidadosamente a imagem do circuito
   - Identifique cada componente
   - Note todos os valores fornecidos
   - Determine o que está sendo solicitado

2. PLANEJAMENTO: Escolha a melhor estratégia
   - Avalie qual teorema/lei é mais eficiente
   - Considere simplificações possíveis
   - Planeje a sequência de cálculos

3. EXECUÇÃO: Resolva passo a passo
   - Aplique as equações fundamentais
   - Mostre cada transformação matemática
   - Mantenha rastreamento das unidades

4. VALIDAÇÃO: Verifique sua resposta
   - Confira se o resultado faz sentido fisicamente
   - Teste com análise dimensional
   - Use regra de três ou proporções quando possível

FORMATO DE RESPOSTA:
### 🔍 Identificação
[O que vejo no circuito]

### 📊 Dados do Problema
[Lista organizada de valores conhecidos]

### 🎯 Objetivo
[O que precisa ser calculado]

### 🛠️ Método Escolhido
[Qual técnica vou usar e justificativa]

### 📐 Resolução Passo a Passo
Passo 1: [descrição + equação + cálculo]
Passo 2: [descrição + equação + cálculo]
...

### ✅ Resposta Final
[Valor numérico com unidade e explicação breve]

### 🔬 Validação
[Verificação do resultado]

REGRAS IMPORTANTES:
- Nunca pule etapas nos cálculos
- Sempre explicite suas suposições
- Use precisão adequada (3-4 algarismos significativos)
- Indique incertezas quando relevante
- Seja transparente sobre limitações da sua análise
"""

        contents = []

        if image_data:
            # Remove o prefixo base64 se existir
            if image_data.startswith("data:image"):
                image_data = image_data.split(",")[1]

            contents.append(
                types.Content(
                    role="user",
                    parts=[
                        types.Part.from_text(text=prompt),
                        types.Part.from_inline_data(
                            mime_type="image/png", data=base64.b64decode(image_data)
                        ),
                    ],
                )
            )
        else:
            contents.append(
                types.Content(
                    role="user",
                    parts=[types.Part.from_text(text=prompt)],
                )
            )

        generate_content_config = types.GenerateContentConfig(
            temperature=0.7,
            thinking_config=types.ThinkingConfig(
                thinking_budget=-1,
            ),
            system_instruction=[types.Part.from_text(text=system_instruction)],
        )

        # Gera resposta
        response = gemini_client.models.generate_content(
            model="gemini-2.5-pro",
            contents=contents,
            config=generate_content_config,
        )

        return {"success": True, "response": response.text}

    except Exception as e:
        return {"success": False, "error": str(e)}


def consolidate_responses(
    gpt_response: str, gemini_response: str, original_prompt: str
) -> dict:
    """Consolida as respostas do GPT e Gemini usando um assistente consolidador"""
    try:
        consolidator = AVAILABLE_ASSISTANTS["Consolidador"]

        # Cria nova thread para consolidação
        thread = client.beta.threads.create()

        consolidation_prompt = f"""Analise estas duas respostas para a pergunta do usuário e crie uma resposta consolidada, 
        aproveitando os melhores insights de ambas:

**Pergunta original:** {original_prompt}

**Resposta do GPT:**
{gpt_response}

**Resposta do Gemini:**
{gemini_response}

Forneça uma resposta consolidada que combine os melhores aspectos de ambas as análises."""

        client.beta.threads.messages.create(
            thread_id=thread.id,
            role="user",
            content=consolidation_prompt,
        )

        # Executa e aguarda conclusão
        run = client.beta.threads.runs.create_and_poll(
            thread_id=thread.id,
            assistant_id=consolidator.id,
        )

        if run.status == "completed":
            messages = client.beta.threads.messages.list(thread_id=thread.id)
            response = messages.data[0].content[0].text.value
            return {"success": True, "response": response}
        else:
            return {"success": False, "error": f"Run status: {run.status}"}

    except Exception as e:
        return {"success": False, "error": str(e)}


# --- Função para processar imagem ---
def process_image(image_file):
    """Processa a imagem e retorna em formato base64 para a API"""
    try:
        # Converte para PIL Image se necessário
        if hasattr(image_file, "read"):
            image = Image.open(image_file)
        else:
            image = image_file

        # Redimensiona se muito grande (para economia de tokens)
        max_size = (1024, 1024)
        if image.size[0] > max_size[0] or image.size[1] > max_size[1]:
            image.thumbnail(max_size, Image.Resampling.LANCZOS)

        # Converte para base64
        buffered = io.BytesIO()
        image.save(buffered, format="PNG")
        img_str = base64.b64encode(buffered.getvalue()).decode()

        return f"data:image/png;base64,{img_str}"
    except Exception as e:
        st.error(f"Erro ao processar imagem: {e}")
        return None


# --- Configuração da Página e Estilos ---
st.set_page_config(
    page_title="Agente POLI",
    layout="centered",
    initial_sidebar_state="expanded",
)

# Aplica o CSS customizado diretamente
st.markdown(
    """
<style>
/* Importa fontes personalizadas */
@import url('https://fonts.googleapis.com/css2?family=Inter:wght@300;400;500;600;700&display=swap');

/* Variáveis CSS personalizadas */
:root {
    --primary-purple: #8c52ff;
    --purple-light: #a06eff;
    --purple-dark: #6b3acc;
    --purple-ultra-light: rgba(140, 82, 255, 0.1);
    --purple-semi-light: rgba(140, 82, 255, 0.2);
    --background-gradient: linear-gradient(135deg, #f8f7ff 0%, #f0ebff 100%);
}

/* Fundo principal com padrão de dados */
.main .block-container {
    background: var(--background-gradient);
    position: relative;
    font-family: 'Inter', sans-serif;
    color: #333333;
}

.main .block-container::before {
    content: '';
    position: fixed;
    top: 0;
    left: 0;
    right: 0;
    bottom: 0;
    background-image: 
        /* Padrão de dados/pontos pequenos */
        radial-gradient(circle at 10% 10%, var(--purple-ultra-light) 1px, transparent 1px),
        radial-gradient(circle at 30% 20%, var(--purple-ultra-light) 1.5px, transparent 1.5px),
        radial-gradient(circle at 50% 15%, var(--purple-ultra-light) 1px, transparent 1px),
        radial-gradient(circle at 70% 25%, var(--purple-ultra-light) 2px, transparent 2px),
        radial-gradient(circle at 90% 10%, var(--purple-ultra-light) 1px, transparent 1px),
        radial-gradient(circle at 85% 30%, var(--purple-ultra-light) 1.5px, transparent 1.5px),
        
        /* Segunda camada de dados */
        radial-gradient(circle at 15% 40%, var(--purple-ultra-light) 1px, transparent 1px),
        radial-gradient(circle at 35% 45%, var(--purple-ultra-light) 2px, transparent 2px),
        radial-gradient(circle at 55% 35%, var(--purple-ultra-light) 1px, transparent 1px),
        radial-gradient(circle at 75% 50%, var(--purple-ultra-light) 1.5px, transparent 1.5px),
        radial-gradient(circle at 95% 45%, var(--purple-ultra-light) 1px, transparent 1px),
        
        /* Terceira camada de dados */
        radial-gradient(circle at 20% 70%, var(--purple-ultra-light) 2px, transparent 2px),
        radial-gradient(circle at 40% 65%, var(--purple-ultra-light) 1px, transparent 1px),
        radial-gradient(circle at 60% 75%, var(--purple-ultra-light) 1.5px, transparent 1.5px),
        radial-gradient(circle at 80% 70%, var(--purple-ultra-light) 1px, transparent 1px),
        
        /* Quarta camada de dados */
        radial-gradient(circle at 25% 90%, var(--purple-ultra-light) 1px, transparent 1px),
        radial-gradient(circle at 45% 95%, var(--purple-ultra-light) 1.5px, transparent 1.5px),
        radial-gradient(circle at 65% 85%, var(--purple-ultra-light) 1px, transparent 1px),
        radial-gradient(circle at 85% 95%, var(--purple-ultra-light) 2px, transparent 2px),
        
        /* Linhas sutis conectando dados */
        linear-gradient(45deg, transparent 45%, var(--purple-ultra-light) 47%, var(--purple-ultra-light) 48%, transparent 50%),
        linear-gradient(-45deg, transparent 45%, var(--purple-ultra-light) 47%, var(--purple-ultra-light) 48%, transparent 50%);
    
    background-size: 
        /* Tamanhos variados para simular dados dispersos */
        80px 80px, 120px 120px, 90px 90px, 110px 110px, 70px 70px, 100px 100px,
        95px 95px, 130px 130px, 85px 85px, 105px 105px, 75px 75px,
        140px 140px, 80px 80px, 115px 115px, 90px 90px,
        125px 125px, 95px 95px, 85px 85px, 135px 135px,
        /* Linhas de conexão */
        200px 200px, 180px 180px;
    
    background-position: 
        /* Posições aleatórias para simular distribuição de dados */
        0 0, 25px 25px, 50px 15px, 75px 35px, 100px 10px, 125px 30px,
        15px 40px, 45px 60px, 70px 45px, 95px 65px, 120px 50px,
        20px 80px, 50px 75px, 80px 85px, 110px 80px,
        30px 100px, 60px 105px, 90px 95px, 120px 110px,
        /* Linhas */
        0 0, 100px 100px;
    
    opacity: 0.4;
    z-index: -1;
    pointer-events: none;
}

/* Adiciona padrão extra de dados flutuantes */
.main .block-container::after {
    content: '';
    position: fixed;
    top: 0;
    left: 0;
    right: 0;
    bottom: 0;
    background-image: 
        /* Pequenos quadrados e retângulos representando dados */
        linear-gradient(45deg, var(--purple-ultra-light) 2px, transparent 2px),
        linear-gradient(-45deg, var(--purple-ultra-light) 1px, transparent 1px),
        /* Círculos maiores ocasionais */
        radial-gradient(circle at 33% 33%, var(--purple-ultra-light) 3px, transparent 3px),
        radial-gradient(circle at 66% 66%, var(--purple-ultra-light) 2.5px, transparent 2.5px);
    background-size: 150px 150px, 180px 180px, 300px 300px, 250px 250px;
    background-position: 0 0, 50px 50px, 75px 75px, 125px 125px;
    opacity: 0.2;
    z-index: -2;
    pointer-events: none;
}

/* Remove a borda padrão do topo do header */
header[data-testid="stHeader"] {
    border-top: none;
    background: linear-gradient(90deg, var(--primary-purple), var(--purple-light));
    height: 4px;
}

/* Estilização da sidebar */
section[data-testid="stSidebar"] {
    background: linear-gradient(180deg, #ffffff 0%, #faf9ff 100%);
    border-right: 2px solid var(--purple-ultra-light);
}

section[data-testid="stSidebar"] > div {
    background: transparent;
}

/* Força texto escuro em todos os elementos */
.stApp {
    color: #333333 !important;
}

/* Botões personalizados */
.stButton > button {
    background: linear-gradient(135deg, var(--primary-purple), var(--purple-light));
    color: white !important;
    border: none;
    border-radius: 12px;
    font-weight: 600;
    font-family: 'Inter', sans-serif;
    transition: all 0.3s ease;
    box-shadow: 0 4px 15px rgba(140, 82, 255, 0.3);
}

.stButton > button:hover {
    background: linear-gradient(135deg, var(--purple-dark), var(--primary-purple));
    transform: translateY(-2px);
    box-shadow: 0 6px 20px rgba(140, 82, 255, 0.4);
}

/* Selectbox personalizado */
.stSelectbox > div > div {
    background-color: white;
    border: 2px solid var(--purple-ultra-light);
    border-radius: 10px;
    transition: border-color 0.3s ease;
    color: #333333 !important;
}

.stSelectbox > div > div:focus-within {
    border-color: var(--primary-purple);
    box-shadow: 0 0 0 3px var(--purple-ultra-light);
}

/* Estiliza os containers das mensagens para se parecerem com os balões de chat */
[data-testid="stChatMessage"] {
    background: rgba(255, 255, 255, 0.9);
    border: 1px solid var(--purple-ultra-light);
    border-radius: 16px;
    padding: 1.5rem;
    margin-bottom: 1rem;
    backdrop-filter: blur(10px);
    box-shadow: 0 4px 20px rgba(140, 82, 255, 0.1);
    transition: all 0.3s ease;
    color: #333333 !important;
}

[data-testid="stChatMessage"]:hover {
    box-shadow: 0 6px 25px rgba(140, 82, 255, 0.15);
    transform: translateY(-1px);
}

/* Mensagens do usuário */
[data-testid="stChatMessage"]:has(img[alt*="user"]) {
    background: linear-gradient(135deg, var(--primary-purple), var(--purple-light));
    color: white !important;
    margin-left: 2rem;
}

/* Mensagens do assistente */
[data-testid="stChatMessage"]:has(img[alt*="assistant"]) {
    background: rgba(255, 255, 255, 0.95);
    margin-right: 2rem;
    color: #333333 !important;
}

/* Estilo para avatares */
img[data-testid="stAvatar"] {
    width: 40px;
    height: 40px;
    border-radius: 50%;
    border: 2px solid var(--primary-purple);
    box-shadow: 0 2px 10px rgba(140, 82, 255, 0.3);
}

/* Input de chat personalizado */
.stChatInput > div {
    background: white;
    border: 2px solid var(--purple-ultra-light);
    border-radius: 15px;
    transition: all 0.3s ease;
}

.stChatInput > div:focus-within {
    border-color: var(--primary-purple);
    box-shadow: 0 0 0 3px var(--purple-ultra-light);
}

/* Títulos personalizados */
h1, h2, h3 {
    color: var(--purple-dark) !important;
    font-family: 'Inter', sans-serif;
    font-weight: 700;
}

/* Texto geral */
p, div, span, label {
    color: #333333 !important;
}

/* Alertas e notificações */
.stAlert {
    background: rgba(255, 255, 255, 0.9);
    border-left: 4px solid var(--primary-purple);
    border-radius: 10px;
    backdrop-filter: blur(10px);
    color: #333333 !important;
}

/* Scrollbar personalizada */
::-webkit-scrollbar {
    width: 8px;
}

::-webkit-scrollbar-track {
    background: var(--purple-ultra-light);
}

::-webkit-scrollbar-thumb {
    background: var(--primary-purple);
    border-radius: 4px;
}

::-webkit-scrollbar-thumb:hover {
    background: var(--purple-dark);
}

/* Animação de loading */
@keyframes pulse {
    0% { opacity: 1; }
    50% { opacity: 0.6; }
    100% { opacity: 1; }
}

.loading-text {
    animation: pulse 1.5s ease-in-out infinite;
}

/* Estilo para área de upload de imagem */
.stFileUploader > div {
    background: white;
    border: 2px dashed var(--purple-ultra-light);
    border-radius: 15px;
    transition: all 0.3s ease;
    padding: 1rem;
}

.stFileUploader > div:hover {
    border-color: var(--primary-purple);
    background: var(--purple-ultra-light);
}

/* Estilo para prévia da imagem */
.image-preview {
    border: 2px solid var(--purple-ultra-light);
    border-radius: 10px;
    padding: 0.5rem;
    margin: 0.5rem 0;
    background: white;
}

/* Estilo para botões de captura */
.capture-button {
    background: linear-gradient(135deg, #4CAF50, #45a049);
    color: white !important;
    border: none;
    border-radius: 12px;
    font-weight: 600;
    font-family: 'Inter', sans-serif;
    transition: all 0.3s ease;
    box-shadow: 0 4px 15px rgba(76, 175, 80, 0.3);
    margin: 0.25rem;
}

.capture-button:hover {
    background: linear-gradient(135deg, #45a049, #4CAF50);
    transform: translateY(-2px);
    box-shadow: 0 6px 20px rgba(76, 175, 80, 0.4);
}

/* Estilo para o botão de anexar (+) */
.attachment-button {
    position: fixed;
    bottom: 80px;
    left: 20px;
    width: 50px;
    height: 50px;
    border-radius: 50%;
    background: linear-gradient(135deg, var(--primary-purple), var(--purple-light));
    color: white !important;
    border: none;
    font-size: 24px;
    font-weight: 600;
    box-shadow: 0 4px 15px rgba(140, 82, 255, 0.4);
    cursor: pointer;
    transition: all 0.3s ease;
    z-index: 1000;
}

.attachment-button:hover {
    transform: scale(1.1);
    box-shadow: 0 6px 20px rgba(140, 82, 255, 0.5);
}

/* Menu de opções de anexo */
.attachment-menu {
    position: fixed;
    bottom: 140px;
    left: 20px;
    background: white;
    border-radius: 15px;
    box-shadow: 0 8px 30px rgba(140, 82, 255, 0.3);
    padding: 1rem;
    z-index: 999;
    min-width: 200px;
    border: 2px solid var(--purple-ultra-light);
}

/* Ajuste para mobile */
@media (max-width: 768px) {
    .attachment-button {
        left: 10px;
        bottom: 70px;
    }
    
    .attachment-menu {
        left: 10px;
        bottom: 130px;
        width: calc(100% - 20px);
        max-width: 300px;
    }
}
</style>
""",
    unsafe_allow_html=True,
)


# --- Inicialização do Cliente OpenAI ---
try:
    client = openai.OpenAI(api_key=st.secrets["OPENAI_API_KEY"])
except Exception as e:
    st.error(
        "Chave da API da OpenAI não encontrada. Por favor, configure seus secrets no Streamlit Cloud.",
        icon="🚨",
    )
    st.stop()


# --- Inicialização do Estado da Sessão ---

if "session_id" not in st.session_state:
    st.session_state.session_id = f"session_{int(time.time())}"

if "messages" not in st.session_state:
    st.session_state.messages = []

if "thread_id" not in st.session_state:
    st.session_state.thread_id = None

if "assistant_key" not in st.session_state:
    st.session_state.assistant_key = DEFAULT_ASSISTANT

if "uploaded_image" not in st.session_state:
    st.session_state.uploaded_image = None

if "image_data" not in st.session_state:
    st.session_state.image_data = None

if "attachment_menu_open" not in st.session_state:
    st.session_state.attachment_menu_open = False

if "camera_active" not in st.session_state:
    st.session_state.camera_active = False


# --- Interface da Sidebar ---
with st.sidebar:
    st.title("💬 Agente POLI")

    if st.button("＋ Nova Conversa", use_container_width=True, key="new_chat_top"):
        st.session_state.messages = []
        st.session_state.thread_id = None
        st.session_state.uploaded_image = None
        st.session_state.image_data = None
        st.session_state.camera_active = False
        st.rerun()

    st.header("⚙️ Configurações")

    # Seletor de Assistente
    assistant_options = {
        key: assistant.name for key, assistant in AVAILABLE_ASSISTANTS.items()
    }

    selected_assistant_key = st.selectbox(
        label="🤖 Assistente:",
        options=assistant_options.keys(),
        format_func=lambda key: assistant_options[key],
        key="selected_assistant",
    )

    # Lógica para confirmar mudança de assistente
    if selected_assistant_key != st.session_state.assistant_key:
        if st.session_state.messages:
            st.warning("Mudar de assistente irá iniciar uma nova conversa.", icon="⚠️")
            if st.button(
                "Confirmar e iniciar nova conversa",
                use_container_width=True,
                key="confirm_assistant_change",
            ):
                st.session_state.assistant_key = selected_assistant_key
                st.session_state.messages = []
                st.session_state.thread_id = None
                st.rerun()
        else:
            st.session_state.assistant_key = selected_assistant_key
            st.rerun()

    # Exibe a descrição do assistente
    assistant_info = AVAILABLE_ASSISTANTS[st.session_state.assistant_key]
    st.markdown(
        f"<small>*{assistant_info.description}*</small>", unsafe_allow_html=True
    )

    st.markdown("---")
    st.markdown("<br>" * 5, unsafe_allow_html=True)

    # Logos
    logo_col1, logo_col2, logo_col3 = st.columns([1, 2, 1])
    with logo_col2:
        st.markdown(
            "<small>🚀 Agente POLI </small>",
            unsafe_allow_html=True,
        )


# --- Interface Principal do Chat ---
assistant_info = AVAILABLE_ASSISTANTS[st.session_state.assistant_key]

# Header do Chat
st.markdown(f"### {assistant_info.name}")

# Indicador de imagem anexada
if st.session_state.image_data:
    st.info("📷 Imagem anexada - será enviada com a próxima mensagem", icon="ℹ️")

# Exibe histórico de mensagens
if not st.session_state.messages:
    st.info(f"Como posso te ajudar hoje como {assistant_info.name}?", icon="👋")

for msg in st.session_state.messages:
    avatar_img = (
        os.path.join(SCRIPT_DIR, "assets", "img", "user.png")
        if msg["role"] == "user"
        else os.path.join(SCRIPT_DIR, "assets", "img", "gpt.png")
    )
    with st.chat_message(msg["role"], avatar=avatar_img):
        if "image" in msg:
            st.image(msg["image"], caption="Imagem enviada", width=300)
        st.markdown(msg["content"])

# --- Menu de Anexos ---
col_attach, col_spacer = st.columns([1, 9])

with col_attach:
    if st.button(
        "➕" if not st.session_state.attachment_menu_open else "✖️",
        key="toggle_attachment_menu",
        help="Anexar imagem",
    ):
        st.session_state.attachment_menu_open = (
            not st.session_state.attachment_menu_open
        )
        st.rerun()

# Menu de opções de anexo
if st.session_state.attachment_menu_open:
    with st.container():
        st.markdown("#### 📎 Anexar Imagem")

        col1, col2 = st.columns(2)

        with col1:
            if st.button("📸 Câmera", use_container_width=True, key="open_camera_main"):
                st.session_state.camera_active = True
                st.session_state.attachment_menu_open = False
                st.rerun()

        with col2:
            uploaded_file = st.file_uploader(
                "📁 Galeria",
                type=["png", "jpg", "jpeg", "gif", "bmp"],
                help="Formatos suportados: PNG, JPG, JPEG, GIF, BMP",
                key="file_uploader_main",
                label_visibility="collapsed",
            )

        if uploaded_file is not None:
            image_data = process_image(uploaded_file)
            if image_data:
                st.session_state.image_data = image_data
                st.session_state.uploaded_image = uploaded_file
                st.session_state.attachment_menu_open = False
                st.success("✅ Imagem anexada!")
                st.rerun()

        st.markdown("---")

# Componente de captura de câmera
if st.session_state.camera_active:
    st.info("📷 Câmera ativada - tire sua foto:", icon="ℹ️")
    camera_image = st.camera_input("📸 Capturar foto", key="camera_input_main")

    col_cancel, col_confirm = st.columns(2)

    with col_cancel:
        if st.button("❌ Cancelar", use_container_width=True, key="cancel_camera_main"):
            st.session_state.camera_active = False
            st.rerun()

    if camera_image is not None:
        image_data = process_image(camera_image)
        if image_data:
            st.session_state.image_data = image_data
            st.session_state.uploaded_image = camera_image
            st.session_state.camera_active = False
            st.success("✅ Foto capturada!")
            time.sleep(1)
            st.rerun()

# Prévia da imagem anexada
if st.session_state.uploaded_image is not None and not st.session_state.camera_active:
    with st.expander("🖼️ Imagem anexada", expanded=False):
        st.image(st.session_state.uploaded_image, use_column_width=True)
        if st.button("🗑️ Remover imagem", key="remove_image_main"):
            st.session_state.uploaded_image = None
            st.session_state.image_data = None
            st.rerun()


# --- Input do Usuário e Processamento ---
if prompt := st.chat_input("Digite sua mensagem aqui..."):
    # Prepara o conteúdo da mensagem
    message_content = prompt
    message_data = {"role": "user", "content": message_content}

    # Se há imagem anexada, adiciona à mensagem
    if st.session_state.image_data:
        message_data["image"] = st.session_state.uploaded_image
        message_content = f"[Imagem anexada] {prompt}"

    # Adiciona e exibe a mensagem do usuário
    st.session_state.messages.append(message_data)
    with st.chat_message(
        "user", avatar=os.path.join(SCRIPT_DIR, "assets", "img", "user.png")
    ):
        if st.session_state.image_data:
            st.image(
                st.session_state.uploaded_image, caption="Imagem enviada", width=300
            )
        st.markdown(prompt)

    # Processa com ambos os modelos em paralelo
    with st.chat_message(
        "assistant", avatar=os.path.join(SCRIPT_DIR, "assets", "img", "gpt.png")
    ):
        status_placeholder = st.empty()
        status_placeholder.info(
            "🔄 Processando com GPT e Gemini em paralelo...", icon="⏳"
        )

        # Executa processamentos em paralelo
        with ThreadPoolExecutor(max_workers=2) as executor:
            gpt_future = executor.submit(
                process_with_gpt,
                prompt,
                st.session_state.image_data,
                st.session_state.thread_id,
            )
            gemini_future = executor.submit(
                process_with_gemini, prompt, st.session_state.image_data
            )

            # Aguarda ambas as respostas
            gpt_result = gpt_future.result()
            gemini_result = gemini_future.result()

        status_placeholder.empty()

        # Verifica se ambos tiveram sucesso
        if gpt_result["success"] and gemini_result["success"]:
            # Atualiza thread_id se necessário
            if "thread_id" in gpt_result:
                st.session_state.thread_id = gpt_result["thread_id"]

            # Consolida as respostas
            consolidation_placeholder = st.empty()
            consolidation_placeholder.info("🔄 Consolidando respostas...", icon="⏳")

            consolidated_result = consolidate_responses(
                gpt_result["response"], gemini_result["response"], prompt
            )

            consolidation_placeholder.empty()

            if consolidated_result["success"]:
                # Mostra a resposta consolidada
                st.markdown("### 📊 Resposta Consolidada")
                st.markdown(consolidated_result["response"])

                # Botões expansíveis para ver respostas individuais
                col1, col2 = st.columns(2)

                with col1:
                    with st.expander("🤖 Ver resposta do GPT"):
                        st.markdown(gpt_result["response"])

                with col2:
                    with st.expander("✨ Ver resposta do Gemini"):
                        st.markdown(gemini_result["response"])

                # Salva no histórico
                st.session_state.messages.append(
                    {
                        "role": "assistant",
                        "content": consolidated_result["response"],
                        "gpt_response": gpt_result["response"],
                        "gemini_response": gemini_result["response"],
                    }
                )
            else:
                st.error(
                    f"Erro na consolidação: {consolidated_result['error']}", icon="🚨"
                )
        else:
            # Mostra erros se houver
            if not gpt_result["success"]:
                st.error(f"Erro no GPT: {gpt_result['error']}", icon="🚨")
            if not gemini_result["success"]:
                st.error(f"Erro no Gemini: {gemini_result['error']}", icon="🚨")

        # Limpa a imagem após envio
        if st.session_state.image_data:
            st.session_state.uploaded_image = None
            st.session_state.image_data = None

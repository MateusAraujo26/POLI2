# --- Imports ---
import base64
import os
import time
import io
from typing import List

import streamlit as st
from openai import OpenAI
from PIL import Image

# --- Configuração da Aplicação ---
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))

# --- Configuração da Página e Estilos ---
st.set_page_config(
    page_title="Agente POLI - Circuitos",
    layout="centered",
    initial_sidebar_state="expanded",
)

# Aplica o CSS customizado
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
    --background-gradient: linear-gradient(135deg, #f8f7ff 0%, #f0ebff 100%);
}

/* Fundo principal */
.main .block-container {
    background: var(--background-gradient);
    font-family: 'Inter', sans-serif;
    color: #333333;
}

/* Header */
header[data-testid="stHeader"] {
    border-top: none;
    background: linear-gradient(90deg, var(--primary-purple), var(--purple-light));
    height: 4px;
}

/* Sidebar */
section[data-testid="stSidebar"] {
    background: linear-gradient(180deg, #ffffff 0%, #faf9ff 100%);
    border-right: 2px solid var(--purple-ultra-light);
}

/* Botões */
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

/* Mensagens de chat */
[data-testid="stChatMessage"] {
    background: rgba(255, 255, 255, 0.9);
    border: 1px solid var(--purple-ultra-light);
    border-radius: 16px;
    padding: 1.5rem;
    margin-bottom: 1rem;
    backdrop-filter: blur(10px);
    box-shadow: 0 4px 20px rgba(140, 82, 255, 0.1);
    transition: all 0.3s ease;
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
}

/* Input de chat */
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

/* Títulos */
h1, h2, h3 {
    color: var(--purple-dark) !important;
    font-family: 'Inter', sans-serif;
    font-weight: 700;
}

/* Alertas */
.stAlert {
    background: rgba(255, 255, 255, 0.9);
    border-left: 4px solid var(--primary-purple);
    border-radius: 10px;
    backdrop-filter: blur(10px);
}

/* Upload de arquivo */
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
</style>
""",
    unsafe_allow_html=True,
)

# --- Inicialização do Cliente OpenAI ---
try:
    client = OpenAI(api_key=st.secrets["OPENAI_API_KEY"])
except Exception as e:
    st.error(
        "⚠️ Chave da API da OpenAI não encontrada. Configure seus secrets.",
        icon="🚨",
    )
    st.stop()


# --- Função para processar imagem ---
def process_image(image_file):
    """Converte imagem para base64"""
    try:
        if hasattr(image_file, "read"):
            image = Image.open(image_file)
        else:
            image = image_file

        # Redimensiona se necessário
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


# --- Função para chamar o agente ---
def call_agent(messages: List[dict]) -> dict:
    """Chama o agente O1-Pro com a nova API Responses"""
    try:
        # Converte mensagens para o formato da API
        api_messages = []
        for msg in messages:
            if msg["role"] == "user":
                content = []
                # Adiciona texto
                content.append({"type": "input_text", "text": msg["content"]})
                # Adiciona imagem se houver
                if "image_data" in msg:
                    content.append(
                        {"type": "input_image", "image_url": {"url": msg["image_data"]}}
                    )
                api_messages.append({"role": "user", "content": content})
            elif msg["role"] == "assistant":
                api_messages.append(
                    {
                        "role": "assistant",
                        "content": [{"type": "output_text", "text": msg["content"]}],
                    }
                )

        # Chama a API Responses
        response = client.chat.completions.create(
            model="o1-pro",
            messages=api_messages,
            reasoning={"effort": "high"},
            store=True,
        )

        # Extrai a resposta
        assistant_message = response.choices[0].message

        # Processa o reasoning se disponível
        reasoning_summary = None
        if hasattr(assistant_message, "reasoning") and assistant_message.reasoning:
            reasoning_summary = (
                assistant_message.reasoning.content
                if hasattr(assistant_message.reasoning, "content")
                else None
            )

        return {
            "success": True,
            "content": assistant_message.content,
            "reasoning": reasoning_summary,
        }

    except Exception as e:
        return {"success": False, "error": str(e)}


# --- Inicialização do Estado da Sessão ---
if "messages" not in st.session_state:
    st.session_state.messages = []

if "uploaded_image" not in st.session_state:
    st.session_state.uploaded_image = None

if "image_data" not in st.session_state:
    st.session_state.image_data = None

if "camera_active" not in st.session_state:
    st.session_state.camera_active = False

# --- Interface da Sidebar ---
with st.sidebar:
    st.title("💬 Agente POLI")
    st.caption("Especialista em Circuitos Elétricos")

    if st.button("＋ Nova Conversa", use_container_width=True, key="new_chat"):
        st.session_state.messages = []
        st.session_state.uploaded_image = None
        st.session_state.image_data = None
        st.session_state.camera_active = False
        st.rerun()

    st.markdown("---")

    st.header("🤖 Sobre o Agente")
    st.markdown(
        """
    **Modelo:** OpenAI O1-Pro  
    **Modo:** High Reasoning Effort
    
    **Especialidades:**
    - ⚡ Análise de circuitos
    - 📊 Cálculos detalhados
    - 🔍 Verificação dimensional
    - 📚 Banco de provas antigas
    """
    )

    st.markdown("---")

    # Estatísticas
    if st.session_state.messages:
        user_msgs = sum(1 for msg in st.session_state.messages if msg["role"] == "user")
        st.metric("📊 Questões Resolvidas", user_msgs)

    st.markdown("<br>" * 2, unsafe_allow_html=True)
    st.markdown(
        "<div style='text-align: center;'><small>🚀 Powered by OpenAI O1-Pro</small></div>",
        unsafe_allow_html=True,
    )

# --- Interface Principal do Chat ---
st.markdown("### 🎯 Resolvedor de Circuitos Elétricos")
st.caption("Análise detalhada com raciocínio aprofundado")

# Indicador de imagem anexada
if st.session_state.image_data:
    st.info("📷 Imagem anexada - será enviada com a próxima mensagem", icon="ℹ️")

# Exibe histórico de mensagens
if not st.session_state.messages:
    st.info(
        "👋 Olá! Envie uma imagem de um circuito elétrico e faça sua pergunta.",
        icon="💡",
    )

for msg in st.session_state.messages:
    avatar_img = (
        os.path.join(SCRIPT_DIR, "assets", "img", "user.png")
        if msg["role"] == "user"
        else os.path.join(SCRIPT_DIR, "assets", "img", "gpt.png")
    )
    with st.chat_message(msg["role"], avatar=avatar_img):
        # Exibe imagem dos bytes salvos (não do objeto de arquivo)
        if "image_bytes" in msg:
            st.image(msg["image_bytes"], caption="Imagem enviada", width=300)
        st.markdown(msg["content"])

        # Mostra raciocínio se disponível
        if msg["role"] == "assistant" and "reasoning" in msg and msg["reasoning"]:
            with st.expander("🧠 Ver raciocínio do modelo"):
                st.markdown(msg["reasoning"])

# --- Menu de Anexos ---
col1, col2 = st.columns([1, 9])

with col1:
    if st.button("📎", key="toggle_attachment", help="Anexar imagem"):
        st.session_state.camera_active = False
        st.rerun()

# Upload de arquivo
uploaded_file = st.file_uploader(
    "📁 Anexar imagem do circuito",
    type=["png", "jpg", "jpeg", "gif", "bmp"],
    help="Formatos suportados: PNG, JPG, JPEG, GIF, BMP",
    key="file_uploader",
)

if uploaded_file is not None:
    image_data = process_image(uploaded_file)
    if image_data:
        st.session_state.image_data = image_data
        st.session_state.uploaded_image = uploaded_file
        st.success("✅ Imagem anexada!")

# Prévia da imagem anexada
if st.session_state.uploaded_image is not None:
    with st.expander("🖼️ Imagem anexada", expanded=False):
        st.image(st.session_state.uploaded_image, use_container_width=True)
        if st.button("🗑️ Remover imagem", key="remove_image"):
            st.session_state.uploaded_image = None
            st.session_state.image_data = None
            st.rerun()

# --- Input do Usuário e Processamento ---
if prompt := st.chat_input("Digite sua pergunta sobre o circuito..."):
    # Prepara a mensagem do usuário
    user_message = {"role": "user", "content": prompt}

    # Salva uma cópia da imagem antes de limpar
    current_image = st.session_state.uploaded_image
    current_image_data = st.session_state.image_data

    # Adiciona imagem se houver
    if current_image_data:
        user_message["image_data"] = current_image_data
        # Converte a imagem para bytes para salvar no histórico
        if hasattr(current_image, "read"):
            current_image.seek(0)  # Volta ao início do arquivo
            image_bytes = current_image.read()
            user_message["image_bytes"] = image_bytes
            current_image.seek(0)  # Volta ao início novamente
        else:
            # Se já for uma imagem PIL
            buffered = io.BytesIO()
            current_image.save(buffered, format="PNG")
            user_message["image_bytes"] = buffered.getvalue()

    # Adiciona ao histórico
    st.session_state.messages.append(user_message)

    # Exibe mensagem do usuário
    with st.chat_message(
        "user", avatar=os.path.join(SCRIPT_DIR, "assets", "img", "user.png")
    ):
        if "image_bytes" in user_message:
            # Exibe imagem dos bytes salvos
            st.image(user_message["image_bytes"], caption="Imagem enviada", width=300)
        st.markdown(prompt)

    # Limpa a imagem ANTES de processar (evita erros de arquivo ausente)
    st.session_state.uploaded_image = None
    st.session_state.image_data = None

    # Processa com o agente
    with st.chat_message(
        "assistant", avatar=os.path.join(SCRIPT_DIR, "assets", "img", "gpt.png")
    ):
        with st.spinner("🤔 Analisando com raciocínio aprofundado..."):
            result = call_agent(st.session_state.messages)

        if result["success"]:
            st.markdown(result["content"])

            # Mostra raciocínio se disponível
            if result.get("reasoning"):
                with st.expander("🧠 Ver raciocínio do modelo"):
                    st.markdown(result["reasoning"])

            # Salva no histórico
            assistant_message = {"role": "assistant", "content": result["content"]}
            if result.get("reasoning"):
                assistant_message["reasoning"] = result["reasoning"]

            st.session_state.messages.append(assistant_message)
        else:
            st.error(f"❌ Erro: {result['error']}", icon="🚨")

    # Rerun para limpar o estado
    st.rerun()

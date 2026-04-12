import os
import io
import base64
from typing import List, Dict, Any, Sequence, cast
import streamlit as st
from openai import OpenAI
from PIL import Image

# -------------------------------------------------------------------
# Configuração básica
# -------------------------------------------------------------------
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))

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


# -------------------------------------------------------------------
# Cliente OpenAI
# -------------------------------------------------------------------
try:
    client = OpenAI(api_key=st.secrets["OPENAI_API_KEY"])
except Exception:
    st.error("⚠️ Configure OPENAI_API_KEY em st.secrets.", icon="🚨")
    st.stop()

# -------------------------------------------------------------------
# Instruções do agente (system/instructions)
# -------------------------------------------------------------------
AGENT_INSTRUCTIONS = """Você é um especialista em análise e resolução de circuitos elétricos com acesso a um banco de provas anteriores.

OBJETIVO:
Analisar circuitos elétricos apresentados em imagens e fornecer soluções detalhadas e precisas.

METODOLOGIA:
1. Identifique todos os componentes do circuito (resistores, capacitores, indutores, fontes, etc.)
2. Reconheça a topologia (série, paralelo, mista)
3. Consulte padrões de provas anteriores quando possível
4. Aplique as leis e teoremas (Ohm, Kirchhoff, Thévenin, Norton, divisor de tensão/corrente etc.)
5. Calcule passo a passo com organização
6. Apresente resultados com unidades corretas

FORMATO DE RESPOSTA:
## Análise do Circuito
## Dados Fornecidos
## Estratégia de Resolução
## Cálculos Detalhados
## Resultado Final
## Verificação

DIRETRIZES:
- Mostrar todos os passos intermediários
- Verificar coerência dimensional
- Notação científica quando apropriado
- Se existir mais de uma abordagem, mencionar
- Ser conciso e exato nos valores finais
"""

# -------------------------------------------------------------------
# Estado
# -------------------------------------------------------------------
if "messages" not in st.session_state:
    st.session_state.messages = []
if "uploaded_image_bytes" not in st.session_state:
    st.session_state.uploaded_image_bytes = None
if "image_data_uri" not in st.session_state:
    st.session_state.image_data_uri = None
if "debug" not in st.session_state:
    st.session_state.debug = False


# -------------------------------------------------------------------
# Utils
# -------------------------------------------------------------------
def process_image_to_data_uri(image: Image.Image) -> str:
    try:
        max_size = (1024, 1024)
        if image.size[0] > max_size[0] or image.size[1] > max_size[1]:
            image.thumbnail(max_size, Image.Resampling.LANCZOS)
        buf = io.BytesIO()
        image.save(buf, format="PNG")
        b64 = base64.b64encode(buf.getvalue()).decode()
        return f"data:image/png;base64,{b64}"
    except Exception as e:
        st.error(f"Erro ao preparar imagem: {e}", icon="🚨")
        return ""


# CORREÇÃO FINAL NA FUNÇÃO build_response_input
def build_response_input(messages: List[Dict[str, Any]]) -> List[Dict[str, Any]]:
    items: List[Dict[str, Any]] = []
    for m in messages:
        if m["role"] == "user":
            content_list: List[Dict[str, Any]] = [
                {"type": "input_text", "text": m["content"]}
            ]

            # CORREÇÃO: Usar a estrutura direta do 'input_image' com a URL (Data URI)
            if "image_data_uri" in m and m["image_data_uri"]:
                content_list.append(
                    {
                        "type": "input_image",
                        "image_url": m[
                            "image_data_uri"
                        ],  # <--- Passa a string do Data URI DIRETAMENTE aqui!
                    }
                )

            # Note que a chave 'content' é a que a responses API espera para o payload de entrada.
            items.append({"role": "user", "content": content_list})

        elif m["role"] == "assistant":
            # Esta parte permanece correta para o histórico
            items.append(
                {
                    "role": "assistant",
                    "content": [{"type": "output_text", "text": m["content"]}],
                }
            )
    return items


def call_agent(messages: List[Dict[str, Any]]) -> Dict[str, Any]:
    try:
        payload = build_response_input(messages)
        response = client.responses.create(
            model="o1-pro",
            instructions=AGENT_INSTRUCTIONS,
            reasoning={"effort": "high"},
            store=True,
            input=cast(Any, payload),
        )
        out_chunks: List[str] = []
        for item in getattr(response, "output", []) or []:
            for c in getattr(item, "content", []) or []:
                if getattr(c, "type", None) == "output_text":
                    out_chunks.append(getattr(c, "text", ""))
        full_text = (
            "\n".join(out_chunks).strip()
            or getattr(response, "output_text", "").strip()
        )

        reasoning_text = None
        if hasattr(response, "reasoning") and response.reasoning:
            r_parts = []
            for rc in getattr(response.reasoning, "content", []) or []:
                if getattr(rc, "type", None) == "output_text":
                    r_parts.append(getattr(rc, "text", ""))
            reasoning_text = "\n".join(r_parts).strip() or None

        return {
            "success": True,
            "content": full_text,
            "reasoning": reasoning_text,
            "raw": response.model_dump() if st.session_state.debug else None,
        }
    except Exception as e:
        return {"success": False, "error": str(e)}


# -------------------------------------------------------------------
# Sidebar
# -------------------------------------------------------------------
with st.sidebar:
    st.title("💬 Agente POLI")
    st.caption("Especialista em Circuitos Elétricos")
    col_a, col_b = st.columns(2)
    with col_a:
        if st.button("＋ Nova", use_container_width=True):
            st.session_state.messages = []
            st.session_state.uploaded_image_bytes = None
            st.session_state.image_data_uri = None
            st.rerun()
    with col_b:
        if st.button(
            "Debug ON" if not st.session_state.debug else "Debug OFF",
            use_container_width=True,
        ):
            st.session_state.debug = not st.session_state.debug
            st.rerun()
    st.markdown("---")
    if st.session_state.messages:
        st.metric(
            "📊 Perguntas",
            sum(1 for m in st.session_state.messages if m["role"] == "user"),
        )
    if st.session_state.debug:
        st.markdown("### Estado")
        st.json(
            {
                "messages_total": len(st.session_state.messages),
                "last_user_has_image": any(
                    m.get("image_file_id")
                    for m in reversed(st.session_state.messages)
                    if m["role"] == "user"
                ),
            }
        )

# -------------------------------------------------------------------
# Header
# -------------------------------------------------------------------
st.markdown("### 🎯 Resolvedor de Circuitos")
st.caption("Envie texto e/ou imagem para obter análise detalhada.")

# -------------------------------------------------------------------
# Histórico
# -------------------------------------------------------------------
for m in st.session_state.messages:
    avatar = (
        os.path.join(SCRIPT_DIR, "assets", "img", "user.png")
        if m["role"] == "user"
        else os.path.join(SCRIPT_DIR, "assets", "img", "gpt.png")
    )
    with st.chat_message(m["role"], avatar=avatar):
        if "image_bytes" in m:
            st.image(m["image_bytes"], caption="Circuito", width=300)
        st.markdown(m["content"])
        if m["role"] == "assistant" and m.get("reasoning"):
            with st.expander("🧠 Raciocínio"):
                st.markdown(m["reasoning"])

# -------------------------------------------------------------------
# Upload de imagem
# -------------------------------------------------------------------
uploaded = st.file_uploader(
    "📁 Imagem do circuito (opcional)",
    type=["png", "jpg", "jpeg"],
    help="Formatos: PNG, JPG, JPEG",
)
if uploaded:
    raw = uploaded.read()
    img = Image.open(io.BytesIO(raw))
    data_uri = process_image_to_data_uri(img)
    if data_uri:
        st.session_state.uploaded_image_bytes = raw
        st.session_state.image_data_uri = data_uri
        st.success("Imagem pronta.")

if st.session_state.uploaded_image_bytes:
    with st.expander("🖼️ Prévia da imagem"):
        st.image(st.session_state.uploaded_image_bytes, use_container_width=True)
        if st.button("Remover imagem"):
            st.session_state.uploaded_image_bytes = None
            st.session_state.image_data_uri = None
            st.rerun()

# Botão para enviar só imagem (sem texto)
send_only_image = False
if st.session_state.image_data_uri:
    if st.button("Enviar só a imagem"):
        send_only_image = True

# -------------------------------------------------------------------
# Entrada de texto
# -------------------------------------------------------------------
prompt = st.chat_input("Digite sua pergunta (ou use 'Enviar só a imagem')...")

should_send = send_only_image or (prompt is not None and prompt.strip() != "")

if should_send:
    if send_only_image and (not prompt or prompt.strip() == ""):
        prompt = "Analise detalhadamente o circuito fornecido na imagem e resolva o(s) problema(s) implicados."

    user_msg: Dict[str, Any] = {"role": "user", "content": prompt}

    # Usa data URI diretamente (Responses API espera image_url, não image_file)
    if st.session_state.image_data_uri:
        user_msg["image_data_uri"] = st.session_state.image_data_uri
        user_msg["image_bytes"] = st.session_state.uploaded_image_bytes

    st.session_state.messages.append(user_msg)

    with st.chat_message(
        "user", avatar=os.path.join(SCRIPT_DIR, "assets", "img", "user.png")
    ):
        if "image_bytes" in user_msg and user_msg["image_bytes"] is not None:
            st.image(user_msg["image_bytes"], caption="Circuito", width=300)
        st.markdown(user_msg["content"])

    with st.chat_message(
        "assistant", avatar=os.path.join(SCRIPT_DIR, "assets", "img", "gpt.png")
    ):
        with st.spinner("Analisando circuito..."):
            result = call_agent(st.session_state.messages)

        if result["success"]:
            st.markdown(result["content"])
            if result.get("reasoning"):
                with st.expander("🧠 Raciocínio do modelo"):
                    st.markdown(result["reasoning"])
            if st.session_state.debug and result.get("raw"):
                with st.expander("🔍 Raw response"):
                    st.json(result["raw"])
            st.session_state.messages.append(
                {
                    "role": "assistant",
                    "content": result["content"],
                    "reasoning": result.get("reasoning"),
                }
            )
        else:
            st.error(f"Erro na requisição: {result['error']}", icon="🚨")
            if st.session_state.debug:
                st.write("Payload enviado:")
                st.json(build_response_input(st.session_state.messages))

    # Limpa imagem após tentativa (mantém se quiser depurar erro)
    st.session_state.uploaded_image_bytes = None
    st.session_state.image_data_uri = None

    # Re-render somente se sucesso (para não perder estado de debug em erro)
    if result.get("success"):
        st.rerun()

import os
import io
import base64
from typing import List, Dict, Any, Union, Optional, Sequence
import streamlit as st
from openai import OpenAI
from PIL import Image

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

try:
    client = OpenAI(api_key=st.secrets["OPENAI_API_KEY"])
except Exception:
    st.error("⚠️ Configure OPENAI_API_KEY em st.secrets.", icon="🚨")
    st.stop()

# Estado
if "messages" not in st.session_state:
    st.session_state.messages = []
if "uploaded_image_bytes" not in st.session_state:
    st.session_state.uploaded_image_bytes = None
if "image_data_uri" not in st.session_state:
    st.session_state.image_data_uri = None


# Utils
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
        st.error(f"Erro imagem: {e}")
        return ""


def upload_image_and_get_file_id(data_uri: str) -> str:
    # data_uri: data:image/png;base64,...
    try:
        if "," in data_uri:
            b64_part = data_uri.split(",", 1)[1]
        else:
            b64_part = data_uri
        raw = base64.b64decode(b64_part)
        bio = io.BytesIO(raw)
        bio.name = "circuit.png"
        up = client.files.create(file=bio, purpose="vision")
        return up.id
    except Exception as e:
        st.error(f"Upload falhou: {e}", icon="🚨")
        return ""


def build_response_input(messages: List[Dict[str, Any]]) -> List[Dict[str, Any]]:
    items: List[Dict[str, Any]] = []
    for m in messages:
        if m["role"] == "user":
            content_parts = [{"type": "input_text", "text": m["content"]}]
            # Se tiver file_id (já enviado)
            if "image_file_id" in m:
                content_parts.append(
                    {
                        "type": "input_image",
                        "image_file": {"file_id": m["image_file_id"]},
                    }
                )
            items.append({"role": "user", "content": content_parts})
        elif m["role"] == "assistant":
            content_parts = [{"type": "output_text", "text": m["content"]}]
            items.append({"role": "assistant", "content": content_parts})
    return items


def call_agent(messages: List[Dict[str, Any]]) -> Dict[str, Any]:
    try:
        response = client.responses.create(
            model="o1-pro",
            reasoning={"effort": "high"},
            store=True,
            input=build_response_input(messages),  # type: ignore
        )
        # Extrai texto de saída
        out_text_parts = []
        for item in getattr(response, "output", []) or []:
            for c in getattr(item, "content", []) or []:
                if c.type == "output_text":
                    out_text_parts.append(c.text)
        full_text = "\n".join(out_text_parts).strip() or getattr(
            response, "output_text", ""
        )

        # Responses API does not return reasoning content
        return {"success": True, "content": full_text, "reasoning": None}
    except Exception as e:
        return {"success": False, "error": str(e)}


# Sidebar
with st.sidebar:
    st.title("💬 Agente POLI")
    st.caption("Especialista em Circuitos Elétricos (O1-Pro)")
    if st.button("＋ Nova Conversa", use_container_width=True):
        st.session_state.messages = []
        st.session_state.uploaded_image_bytes = None
        st.session_state.image_data_uri = None
        st.rerun()
    st.markdown("---")
    if st.session_state.messages:
        count = sum(1 for m in st.session_state.messages if m["role"] == "user")
        st.metric("📊 Questões Resolvidas", count)

st.markdown("### 🎯 Resolvedor de Circuitos Elétricos")
st.caption("Use texto e opcionalmente imagem do circuito.")

# Histórico
for m in st.session_state.messages:
    avatar = (
        os.path.join(SCRIPT_DIR, "assets", "img", "user.png")
        if m["role"] == "user"
        else os.path.join(SCRIPT_DIR, "assets", "img", "gpt.png")
    )
    with st.chat_message(m["role"], avatar=avatar):
        if "image_bytes" in m and m["image_bytes"] is not None:
            st.image(m["image_bytes"], caption="Circuito", width=300)
        st.markdown(m["content"])
        if m["role"] == "assistant" and m.get("reasoning"):
            with st.expander("🧠 Raciocínio"):
                st.markdown(m["reasoning"])

# Upload imagem
uploaded = st.file_uploader(
    "📁 Anexar imagem do circuito",
    type=["png", "jpg", "jpeg"],
    help="Formatos suportados.",
)
if uploaded:
    raw_bytes = uploaded.read()
    st.session_state.uploaded_image_bytes = raw_bytes
    img = Image.open(io.BytesIO(raw_bytes))
    st.session_state.image_data_uri = process_image_to_data_uri(img)
    st.success("Imagem pronta para envio.")
if st.session_state.uploaded_image_bytes:
    with st.expander("🖼️ Prévia da imagem", expanded=False):
        st.image(st.session_state.uploaded_image_bytes, use_container_width=True)
        if st.button("Remover imagem"):
            st.session_state.uploaded_image_bytes = None
            st.session_state.image_data_uri = None
            st.rerun()

# Entrada usuário
prompt = st.chat_input("Digite sua pergunta...")
if prompt:
    user_msg: Dict[str, Union[str, bytes, Optional[bytes], Any]] = {
        "role": "user",
        "content": prompt,
    }
    # Se houver imagem anexada, subir agora e guardar file_id
    if st.session_state.image_data_uri:
        file_id = upload_image_and_get_file_id(st.session_state.image_data_uri)
        if file_id:
            user_msg["image_file_id"] = file_id
            user_msg["image_bytes"] = st.session_state.uploaded_image_bytes
    st.session_state.messages.append(user_msg)

    with st.chat_message(
        "user", avatar=os.path.join(SCRIPT_DIR, "assets", "img", "user.png")
    ):
        if "image_bytes" in user_msg and user_msg["image_bytes"] is not None:
            st.image(user_msg["image_bytes"], caption="Circuito", width=300)
        st.markdown(prompt)

    # Chamada ao modelo
    with st.chat_message(
        "assistant", avatar=os.path.join(SCRIPT_DIR, "assets", "img", "gpt.png")
    ):
        with st.spinner("Analisando..."):
            result = call_agent(st.session_state.messages)
        if result["success"]:
            st.markdown(result["content"])
            if result.get("reasoning"):
                with st.expander("🧠 Raciocínio"):
                    st.markdown(result["reasoning"])
            st.session_state.messages.append(
                {
                    "role": "assistant",
                    "content": result["content"],
                    "reasoning": result.get("reasoning"),
                }
            )
        else:
            st.error(f"Erro: {result['error']}", icon="🚨")

    # Limpa imagem após envio
    st.session_state.uploaded_image_bytes = None
    st.session_state.image_data_uri = None
    st.rerun()

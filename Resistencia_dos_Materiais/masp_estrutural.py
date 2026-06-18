import streamlit as st
from matplotlib.figure import Figure
import matplotlib.patches as patches
import numpy as np
import io
import base64

# ══════════════════════════════════════════════════════════════
#  MASP — Esforços Solicitantes na Viga Principal
#  PEF-3208 · Fundamentos de Mecânica das Estruturas · POLI-USP
# ══════════════════════════════════════════════════════════════


def formatar_numero(valor, dec=2):
    try:
        val = float(valor)
    except (ValueError, TypeError):
        return str(valor)
    
    if val.is_integer():
        return f"{int(val):,}".replace(",", ".")
    
    s = f"{val:,.{dec}f}"
    if "." in s:
        s = s.rstrip("0").rstrip(".")
    if "." not in s:
        return s.replace(",", ".")
    
    partes = s.split(".")
    parte_inteira = partes[0].replace(",", ".")
    parte_decimal = partes[1]
    return f"{parte_inteira},{parte_decimal}"


def formatar_latex(valor, dec=2):
    return formatar_numero(valor, dec).replace(",", "{,}")


def calcular_reacoes(q, P, a, L):
    """Reações de apoio pelas 3 equações de equilíbrio."""
    HD = 0.0  # ∑Fx = 0
    RD = (q * L**2 / 2 + P * a) / L  # ∑Mc = 0
    RC = q * L + P - RD  # ∑Fy = 0
    return round(RC, 3), round(RD, 3), round(HD, 3)


def esforcos_solicitantes(x, RC, q, P, a):
    """
    N, V, M na seção x (subestrutura da esquerda).
    Convenção PEF-3208:
      N > 0 → saindo da seção (tração)
      V > 0 → gira a subestrutura no sentido horário
      M > 0 → traciona fibras inferiores (diagrama desenhado abaixo)
    """
    N = np.zeros_like(x)  # sem força horizontal na viga
    V = np.where(x <= a, RC - q * x, RC - q * x - P)
    M = np.where(x <= a, RC * x - 0.5 * q * x**2, RC * x - 0.5 * q * x**2 - P * (x - a))
    return N, V, M


def plot_analise(q, P, a, L, RC, RD, HD):
    xs = np.linspace(0, L, 500)
    N, V, M = esforcos_solicitantes(xs, RC, q, P, a)

    V_C = RC  # cortante imediatamente após C
    V_D = RC - q * L - P  # cortante imediatamente antes de D
    M_max_val = float(np.max(M))
    x_Mmax = float(xs[np.argmax(M)])

    # Criamos figuras separadas para o modelo e cada esforço
    # Usando Figure em vez de plt.subplots() para evitar memory leaks
    fig0 = Figure(figsize=(11, 4.4))
    ax0 = fig0.subplots()
    fig1 = Figure(figsize=(11, 3.2))
    ax1 = fig1.subplots()
    fig2 = Figure(figsize=(11, 3.2))
    ax2 = fig2.subplots()
    fig3 = Figure(figsize=(11, 3.2))
    ax3 = fig3.subplots()

    figuras = (fig0, fig1, fig2, fig3)
    axes = (ax0, ax1, ax2, ax3)

    for fig in figuras:
        fig.patch.set_facecolor("white")
    for ax in axes:
        ax.set_facecolor("white")
        ax.tick_params(colors="#333333", labelsize=9)
        for sp in ax.spines.values():
            sp.set_edgecolor("#cccccc")
        ax.grid(True, color="#e0e0e0", lw=0.6, ls="--")

    COR_EST = "#424242"
    COR_N = "#00796b"
    COR_V = "#1976d2"
    COR_M = "#d32f2f"
    COR_TXT = "#212121"
    COR_MUT = "#666666"
    COR_REA = "#7b1fa2"  # Purple for reaction forces

    # ── Painel 1 — Esquema estrutural ────────────────────────
    ax0 = axes[0]
    ax0.set_title(
        "Modelo Estrutural — Viga principal do MASP (trecho CD)",
        color=COR_TXT,
        fontsize=12,
        fontweight="bold",
        pad=10,
    )

    # viga
    ax0.plot([0, L], [0, 0], "-", color=COR_EST, lw=5, solid_capstyle="round")

    # apoio simples em C (triângulo + roletes)
    tri_C = patches.Polygon(
        [[0, 0], [-2, -3], [2, -3]], closed=True, fc="#c8e6c9", ec="#2e7d32", lw=1.5
    )
    ax0.add_patch(tri_C)
    for dx in [-0.8, 0.8]:
        ax0.add_patch(patches.Circle((dx, -3.5), 0.5, fc="#c8e6c9", ec="#2e7d32"))
    ax0.plot([-3, 3], [-4, -4], "-", color="#888888", lw=1.5)

    # apoio fixo em D (triângulo preenchido + hachura)
    tri_D = patches.Polygon(
        [[L, 0], [L - 2, -3], [L + 2, -3]],
        closed=True,
        fc="#bbdefb",
        ec="#1565c0",
        lw=1.5,
    )
    ax0.add_patch(tri_D)
    ax0.plot([L - 3, L + 3], [-3, -3], "-", color="#888888", lw=1.5)
    for i in range(7):
        xi = (L - 3) + i * 1.0
        ax0.plot([xi, xi - 0.7], [-3, -4], "-", color="#888888", lw=0.8)

    # carga distribuída
    n_arr = 14
    for i in range(n_arr + 1):
        xi = i * L / n_arr
        ax0.annotate(
            "",
            xy=(xi, 0),
            xytext=(xi, 4.5),
            arrowprops=dict(
                arrowstyle="->,head_width=0.5,head_length=0.9",
                fc="#e53935",
                ec="#e53935",
                lw=1.2,
            ),
        )
    ax0.plot([0, L], [4.5, 4.5], "-", color="#e53935", lw=1.5)
    ax0.text(
        L / 2,
        5.2,
        f"q = {formatar_numero(q, 1)} kN/m  (carga distribuída uniforme)",
        ha="center",
        color="#e53935",
        fontsize=9,
        fontweight="bold",
    )

    # carga concentrada
    if P > 0:
        ax0.annotate(
            "",
            xy=(a, 0),
            xytext=(a, 8),
            arrowprops=dict(
                arrowstyle="->,head_width=1.0,head_length=1.3",
                fc="#ff8f00",
                ec="#ff8f00",
                lw=2.5,
            ),
        )
        ax0.text(
            a,
            8.7,
            f"P = {formatar_numero(P, 0)} kN",
            ha="center",
            color="#ff8f00",
            fontsize=9,
            fontweight="bold",
        )
    else:
        # P == 0: mostra o ponto de aplicação na viga
        ax0.plot(a, 0, "o", color="#ff8f00", ms=8, zorder=5)
        ax0.text(
            a,
            -1.8,
            "P = 0 kN",
            ha="center",
            va="top",
            color="#ff8f00",
            fontsize=8,
            fontweight="bold",
        )

    # reações (desenhadas abaixo dos apoios para não sobrepor)
    ax0.annotate(
        "",
        xy=(0, -4),
        xytext=(0, -8),
        arrowprops=dict(
            arrowstyle="->,head_width=1.0,head_length=1.2",
            fc=COR_REA,
            ec=COR_REA,
            lw=2.5,
        ),
    )
    ax0.text(
        0,
        -9.5,
        f"RC = {formatar_numero(RC, 2)} kN",
        color=COR_REA,
        fontsize=9,
        fontweight="bold",
        ha="center",
    )

    ax0.annotate(
        "",
        xy=(L, -3),
        xytext=(L, -8),
        arrowprops=dict(
            arrowstyle="->,head_width=1.0,head_length=1.2",
            fc=COR_REA,
            ec=COR_REA,
            lw=2.5,
        ),
    )
    ax0.text(
        L,
        -9.5,
        f"RD = {formatar_numero(RD, 2)} kN",
        color=COR_REA,
        fontsize=9,
        fontweight="bold",
        ha="center",
    )

    # rótulos C e D
    for px, nome in [(0, "C  (apoio simples)"), (L, "D  (apoio fixo)")]:
        ax0.plot(px, 0, "o", color=COR_EST, ms=7)
        ax0.text(px + (1 if px == 0 else -14), 1.2, nome, color=COR_TXT, fontsize=9)

    # cota L
    ax0.annotate(
        "",
        xy=(0, -11.5),
        xytext=(L, -11.5),
        arrowprops=dict(arrowstyle="<->", color=COR_MUT, lw=1),
    )
    ax0.text(L / 2, -13, f"L = {formatar_numero(L, 0)} m", ha="center", color=COR_MUT, fontsize=9)

    if P > 0:
        # cota a
        ax0.annotate(
            "",
            xy=(0, -14.5),
            xytext=(a, -14.5),
            arrowprops=dict(arrowstyle="<->", color="#ff8f00", lw=0.9),
        )
        ax0.text(a / 2, -16, f"a = {formatar_numero(a, 1)} m", ha="center", color="#ff8f00", fontsize=8)

    ax0.set_xlim(-8, L + 8)
    ax0.set_ylim(-18, 11)
    ax0.set_aspect("equal")
    ax0.axis("off")
    ax0.grid(False)

    # ── Painéis 2, 3, 4 — Diagramas N, V, M ─────────────────
    dados = [
        (
            axes[1],
            N,
            COR_N,
            "Diagrama de Força Normal  N(x)",
            "N [kN]",
            "N > 0: tração (sai da seção)\nN < 0: compressão (entra na seção)",
        ),
        (
            axes[2],
            V,
            COR_V,
            "Diagrama de Força Cortante  V(x)",
            "V [kN]",
            "V > 0: gira subestrutura no sentido horário\nV < 0: sentido anti-horário",
        ),
        (
            axes[3],
            M,
            COR_M,
            "Diagrama de Momento Fletor  M(x)",
            "M [kN·m]",
            "M > 0: traciona fibras inferiores → diagrama traçado ABAIXO",
        ),
    ]

    for ax, vals, cor, titulo, ylabel, convencao in dados:
        ax.set_title(titulo, color=COR_TXT, fontsize=11, fontweight="bold", pad=6)

        if titulo.startswith("Diagrama de Momento"):
            # Convenção PEF-3208: M>0 desenhado abaixo → inverter eixo y
            plot_vals = -vals
        else:
            plot_vals = vals

        ax.fill_between(xs, plot_vals, 0, where=plot_vals >= 0, color=cor, alpha=0.15)
        ax.fill_between(
            xs, plot_vals, 0, where=plot_vals < 0, color="#e53935", alpha=0.15
        )
        ax.plot(xs, plot_vals, color=cor, lw=2)
        ax.axhline(0, color=COR_MUT, lw=0.8)
        ax.set_xlabel("x [m]  (origem em C)", color=COR_MUT, fontsize=9)
        ax.set_ylabel(ylabel, color=COR_MUT, fontsize=9)

        # anotação de convenção
        ax.text(
            0.01,
            0.04,
            convencao,
            transform=ax.transAxes,
            color=COR_MUT,
            fontsize=7.5,
            va="bottom",
            style="italic",
        )

        # valores relevantes
        if titulo.startswith("Diagrama de Força Cortante"):
            ax.text(
                0.99,
                0.97,
                f"V(0⁺) = {formatar_numero(V_C, 2)} kN  |  V(L⁻) = {formatar_numero(V_D, 2)} kN",
                transform=ax.transAxes,
                ha="right",
                va="top",
                color=COR_V,
                fontsize=9,
                fontweight="bold",
            )
        elif titulo.startswith("Diagrama de Momento"):
            ax.text(
                0.99,
                0.97,
                f"M_máx = {formatar_numero(M_max_val, 1)} kN·m  @ x = {formatar_numero(x_Mmax, 2)} m",
                transform=ax.transAxes,
                ha="right",
                va="top",
                color=COR_M,
                fontsize=9,
                fontweight="bold",
            )
            ax.invert_yaxis()
            ax.set_ylabel("M [kN·m]\n(positivo ↓)", color=COR_MUT, fontsize=9)

    for fig in figuras:
        fig.tight_layout(pad=1.5)
    return figuras, M_max_val, x_Mmax, V_C, V_D


@st.cache_data(max_entries=50, show_spinner=False)
def gerar_diagramas_svg(q, P, a, L, RC, RD, HD):
    figuras, M_max_val, x_Mmax, V_C, V_D = plot_analise(q, P, a, L, RC, RD, HD)
    
    svgs = []
    for fig in figuras:
        buf = io.BytesIO()
        fig.savefig(buf, format="svg", bbox_inches="tight")
        svgs.append(buf.getvalue().decode('utf-8'))
        fig.clear()
        
    return svgs, M_max_val, x_Mmax, V_C, V_D


def renderizar_svg(svg_str):
    b64 = base64.b64encode(svg_str.encode('utf-8')).decode("utf-8")
    html = f'<img src="data:image/svg+xml;base64,{b64}" width="100%"/>'
    st.markdown(html, unsafe_allow_html=True)


# ══════════════════════════════════════════════════════════════
#  Interface Streamlit
# ══════════════════════════════════════════════════════════════

st.set_page_config(
    page_title="MASP — Esforços Solicitantes", layout="wide"
)

st.title("MASP — Esforços Solicitantes na Viga Principal")
st.markdown("""
Esta simulação analisa a **viga principal (trecho CD)** do MASP de forma isolada, permitindo visualizar de maneira direta seus diagramas de esforços solicitantes.
""")

# ── Sidebar ──────────────────────────────────────────────────
st.sidebar.header("Parâmetros de Simulação")

st.sidebar.markdown("**Geometria**")
L = st.sidebar.number_input(
    "L [m]  — comprimento da viga",
    value=70.0,
    step=1.0,
    min_value=5.0,
    help="Vão do MASP: 70 m",
)

st.sidebar.markdown("**Carregamento distribuído uniforme**")
q = st.sidebar.number_input(
    "q [kN/m]  — carga distribuída em toda a viga",
    value=459.0,
    step=10.0,
    min_value=0.0,
    help="Valor de referência: 459 kN/m (viga + laje)",
)

st.sidebar.markdown("**Carregamento concentrado**")
P = st.sidebar.number_input(
    "P [kN]  — carga concentrada",
    value=0.0,
    step=500.0,
    min_value=0.0,
    help="Carga pontual",
)
a = st.sidebar.slider(
    "a [m]  — posição de P a partir de C",
    min_value=0.0,
    max_value=float(L),
    value=min(35.0, float(L)),
    step=0.5,
)

# ── Reações de apoio ─────────────────────────────────────────
RC, RD, HD = calcular_reacoes(q, P, a, L)

# ── Equivalente mecânico da carga distribuída ─────────────────
F_eq = q * L  # módulo: área do diagrama de carga
x_eq = L / 2.0  # ponto de aplicação: centro (carga uniforme)

# ── 1. Visualização da Simulação (Diagramas) ─────────────────
with st.spinner("Gerando diagramas..."):
    svgs, M_max, x_Mmax, V_C, V_D = gerar_diagramas_svg(q, P, a, L, RC, RD, HD)
    renderizar_svg(svgs[0])

# ── 2. Equações e Diagramas de Esforços Solicitantes ──────────
st.markdown("### Diagramas e Equações de Esforços Solicitantes")

# 1. Esforço Normal
st.markdown("#### 1. Esforço Normal $N(x)$")
if P == 0:
    st.markdown(rf"$$N(x) = 0 \quad [kN]$$")
else:
    st.markdown(rf"""
    **Para o trecho $0 \le x \le {formatar_latex(a, 1)}\text{{ m}}$ e para ${formatar_latex(a, 1)}\text{{ m}} < x \le {formatar_latex(L, 1)}\text{{ m}}$:**
    $$N(x) = 0 \quad [kN]$$
    """)
renderizar_svg(svgs[1])

# 2. Esforço Cortante
st.markdown("#### 2. Esforço Cortante $V(x)$")
if P == 0:
    st.markdown(rf"$$V(x) = {formatar_latex(RC)} - {formatar_latex(q)} \cdot x \quad [kN]$$")
else:
    st.markdown(rf"""
    **Para o trecho $0 \le x \le {formatar_latex(a, 1)}\text{{ m}}$:**
    $$V(x) = {formatar_latex(RC)} - {formatar_latex(q)} \cdot x \quad [kN]$$

    **Para o trecho ${formatar_latex(a, 1)}\text{{ m}} < x \le {formatar_latex(L, 1)}\text{{ m}}$:**
    $$V(x) = {formatar_latex(RC)} - {formatar_latex(q)} \cdot x - {formatar_latex(P)} \quad [kN]$$
    """)
renderizar_svg(svgs[2])

# 3. Momento Fletor
st.markdown("#### 3. Momento Fletor $M(x)$")
if P == 0:
    st.markdown(rf"$$M(x) = {formatar_latex(RC)} \cdot x - {formatar_latex(q/2)} \cdot x^2 \quad [kN\cdot m]$$")
else:
    st.markdown(rf"""
    **Para o trecho $0 \le x \le {formatar_latex(a, 1)}\text{{ m}}$:**
    $$M(x) = {formatar_latex(RC)} \cdot x - {formatar_latex(q/2)} \cdot x^2 \quad [kN\cdot m]$$

    **Para o trecho ${formatar_latex(a, 1)}\text{{ m}} < x \le {formatar_latex(L, 1)}\text{{ m}}$:**
    $$M(x) = {formatar_latex(RC)} \cdot x - {formatar_latex(q/2)} \cdot x^2 - {formatar_latex(P)} \cdot (x - {formatar_latex(a, 1)}) \quad [kN\cdot m]$$
    """)
renderizar_svg(svgs[3])

# ── 3. Sobre a Estrutura ──────────────────────────────────────
st.markdown("---")
st.markdown("### Sobre a Estrutura Modelada")
st.markdown(r"""
O edifício do MASP (Museu de Arte de São Paulo) é estruturalmente composto por um pórtico tridimensional complexo -- composto por pilares, vigas de cobertura, tirantes e uma laje suspensa. 

Nesta simulação educacional, **isolamos apenas a viga principal (trecho CD)**. Essa simplificação, modelando as conexões com os pilares como apoios simples e fixos nos pontos C e D, nos permite analisar diretamente os diagramas de força normal, cortante e momento fletor da viga de forma isolada e didática.

A carga uniforme de projeto de referência de $459\text{ kN/m}$ engloba o peso próprio da grande viga superior de concreto $(132\text{ kN/m})$ somado à carga transmitida pelos tirantes da laje inferior do vão livre $(327\text{ kN/m})$.
""")
st.markdown("---")
st.caption(
    "PEF-3208 · Fundamentos de Mecânica das Estruturas  --  POLI-USP · MASP"
)

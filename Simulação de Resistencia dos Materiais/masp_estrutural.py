import streamlit as st
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import numpy as np

# ══════════════════════════════════════════════════════════════
#  MASP — Esforços Solicitantes na Viga Principal
#  PEF-3208 · Fundamentos de Mecânica das Estruturas · POLI-USP
# ══════════════════════════════════════════════════════════════


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
    xs = np.linspace(0, L, 2000)
    N, V, M = esforcos_solicitantes(xs, RC, q, P, a)

    V_C = RC  # cortante imediatamente após C
    V_D = RC - q * L - P  # cortante imediatamente antes de D
    M_max_val = float(np.max(M))
    x_Mmax = float(xs[np.argmax(M)])

    fig, axes = plt.subplots(
        4, 1, figsize=(11, 14), gridspec_kw={"height_ratios": [2.2, 1, 1, 1]}
    )
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
        "Modelo Estrutural — Viga Principal do MASP (trecho CD)",
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
        f"q = {q:.1f} kN/m  (carga distribuída uniforme)",
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
            f"P = {P:.0f} kN\n(a = {a:.1f} m de C)",
            ha="center",
            color="#ff8f00",
            fontsize=9,
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
        f"RC = {RC:.2f} kN",
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
        f"RD = {RD:.2f} kN",
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
    ax0.text(L / 2, -13, f"L = {L:.0f} m", ha="center", color=COR_MUT, fontsize=9)

    if P > 0:
        # cota a
        ax0.annotate(
            "",
            xy=(0, -14.5),
            xytext=(a, -14.5),
            arrowprops=dict(arrowstyle="<->", color="#ff8f00", lw=0.9),
        )
        ax0.text(a / 2, -16, f"a = {a:.1f} m", ha="center", color="#ff8f00", fontsize=8)

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
                f"V(0⁺) = {V_C:.2f} kN  |  V(L⁻) = {V_D:.2f} kN",
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
                f"M_máx = {M_max_val:,.1f} kN·m  @ x = {x_Mmax:.2f} m",
                transform=ax.transAxes,
                ha="right",
                va="top",
                color=COR_M,
                fontsize=9,
                fontweight="bold",
            )
            ax.invert_yaxis()
            ax.set_ylabel("M [kN·m]\n(positivo ↓)", color=COR_MUT, fontsize=9)

    plt.tight_layout(pad=2.5)
    return fig, M_max_val, x_Mmax, V_C, V_D


# ══════════════════════════════════════════════════════════════
#  Interface Streamlit
# ══════════════════════════════════════════════════════════════

st.set_page_config(
    page_title="MASP — Esforços Solicitantes", layout="wide", page_icon="🏛️"
)

st.title("🏛️ MASP — Esforços Solicitantes na Viga Principal")
st.markdown("""
Esta simulação analisa a **Viga Principal (trecho CD)** do MASP de forma isolada, permitindo visualizar de maneira direta seus diagramas de esforços solicitantes.
""")

# ── Sidebar ──────────────────────────────────────────────────
st.sidebar.header("Parâmetros de Simulação")

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
    max_value=70.0,
    value=35.0,
    step=0.5,
)

st.sidebar.markdown("**Geometria**")
L = st.sidebar.number_input(
    "L [m]  — comprimento da viga",
    value=70.0,
    step=1.0,
    min_value=5.0,
    help="Vão do MASP: 70 m",
)
a = min(a, float(L))

# ── Reações de apoio ─────────────────────────────────────────
RC, RD, HD = calcular_reacoes(q, P, a, L)

# ── Equivalente mecânico da carga distribuída ─────────────────
F_eq = q * L  # módulo: área do diagrama de carga
x_eq = L / 2.0  # ponto de aplicação: centro (carga uniforme)

# ── 1. Visualização da Simulação (Diagramas) ─────────────────
with st.spinner("Gerando diagramas..."):
    fig, M_max, x_Mmax, V_C, V_D = plot_analise(q, P, a, L, RC, RD, HD)
    st.pyplot(fig)

# ── 2. Equações Finais de Esforços Solicitantes ──────────────
st.markdown("### 📐 Equações dos Esforços Solicitantes")

if P == 0:
    st.markdown(rf"""
    $$N(x) = 0 \quad [kN]$$
    $$V(x) = {RC:.2f} - {q:.2f} \cdot x \quad [kN]$$
    $$M(x) = {RC:.2f} \cdot x - {q/2:.2f} \cdot x^2 \quad [kN\cdot m]$$
    """)
else:
    st.markdown(rf"""
    **Para o trecho $0 \le x \le {a:.1f}\text{{ m}}$:**
    $$N(x) = 0 \quad [kN]$$
    $$V(x) = {RC:.2f} - {q:.2f} \cdot x \quad [kN]$$
    $$M(x) = {RC:.2f} \cdot x - {q/2:.2f} \cdot x^2 \quad [kN\cdot m]$$

    **Para o trecho ${a:.1f}\text{{ m}} < x \le {L:.1f}\text{{ m}}$:**
    $$N(x) = 0 \quad [kN]$$
    $$V(x) = {RC:.2f} - {q:.2f} \cdot x - {P:.2f} \quad [kN]$$
    $$M(x) = {RC:.2f} \cdot x - {q/2:.2f} \cdot x^2 - {P:.2f} \cdot (x - {a:.1f}) \quad [kN\cdot m]$$
    """)

# ── 3. Sobre a Estrutura ──────────────────────────────────────
st.markdown("---")
st.markdown("### 🏛️ Sobre a Estrutura Modelada")
st.markdown(f"""
**Estudo de Caso — MASP:**
O edifício do MASP (Museu de Arte de São Paulo) é estruturalmente composto por um pórtico tridimensional complexo (composto por pilares, vigas de cobertura, tirantes e uma laje suspensa). 

Nesta simulação educacional, **isolamos apenas a Viga Principal (trecho CD)**. Em Resistência dos Materiais, essa simplificação (modelando as conexões com os pilares como apoios simples e fixos nos pontos C e D) nos permite analisar diretamente os diagramas de força normal, cortante e momento fletor da viga de forma isolada e didática.

A carga uniforme de projeto de referência de $459\text{{ kN/m}}$ engloba o peso próprio da grande viga superior de concreto ($132\text{{ kN/m}}$) somado à carga transmitida pelos tirantes da laje inferior do vão livre ($327\text{{ kN/m}}$).
""")
st.markdown("---")
st.caption(
    "PEF-3208 · Fundamentos de Mecânica das Estruturas · POLI-USP · Estudo de Caso: MASP"
)

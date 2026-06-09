import streamlit as st
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import numpy as np
import sympy as sp
import io

# ─────────────────────────────────────────────
# MASP — Análise Estrutural Interativa
# Pórtico plano isostático simplificado
# ─────────────────────────────────────────────

def resolver_portico(q_ef, q_viga, N_pret, P_pont, x_pont, L=70.0, h=11.0):
    q_total = q_ef + q_viga
    R_Av = q_total * L / 2 + P_pont * (L - x_pont) / L
    R_Bv = q_total * L / 2 + P_pont * x_pont / L
    N_pil_esq = -R_Av
    N_pil_dir = -R_Bv
    N_viga = -N_pret

    xs = np.linspace(0, L, 500)
    Vnum = np.where(xs <= x_pont,
                    R_Av - q_total * xs,
                    R_Av - q_total * xs - P_pont)
    Mnum = np.where(xs <= x_pont,
                    R_Av * xs - 0.5 * q_total * xs**2,
                    R_Av * xs - 0.5 * q_total * xs**2 - P_pont * (xs - x_pont))

    return dict(
        R_Av=round(R_Av, 2), R_Bv=round(R_Bv, 2),
        N_pil_esq=round(N_pil_esq, 2), N_pil_dir=round(N_pil_dir, 2),
        N_viga=round(N_viga, 2),
        xs=xs, Vnum=Vnum, Mnum=Mnum,
        M_max=round(float(np.max(Mnum)), 2),
        V_max=round(float(np.max(np.abs(Vnum))), 2),
        x_Mmax=round(float(xs[np.argmax(Mnum)]), 2)
    )


def capacidade_ruptura(res):
    fcd   = 23_333   # kN/m²
    b, d  = 1.5, 3.0
    tau_d = 700      # kN/m²
    Mr = 0.68 * fcd * b * d**2
    Vr = tau_d * b * d
    Nr = fcd * b * d

    uso_M = abs(res["M_max"]) / Mr * 100
    uso_V = max(abs(res["R_Av"]), abs(res["R_Bv"])) / Vr * 100
    uso_N = abs(res["N_viga"]) / Nr * 100

    return dict(Mr=round(Mr, 0), Vr=round(Vr, 0), Nr=round(Nr, 0),
                uso_M=round(uso_M, 1), uso_V=round(uso_V, 1), uso_N=round(uso_N, 1))


def plot_masp(res, cap, q_ef, q_viga, N_pret, P_pont, x_pont, L=70.0, h=11.0):
    cor_bg  = "#0f0f0f"
    cor_est = "#e0c87a"
    cor_V   = "#4fc3f7"
    cor_M   = "#ef9a9a"
    cor_txt = "#f0f0f0"
    cor_mut = "#aaaaaa"
    cor_rup = "#ff5252"
    cor_ok  = "#69f0ae"

    fig = plt.figure(figsize=(14, 16))
    fig.patch.set_facecolor(cor_bg)

    def axstyle(ax, title=""):
        ax.set_facecolor("#1a1a1a")
        ax.tick_params(colors=cor_mut, labelsize=9)
        for sp_ in ax.spines.values():
            sp_.set_edgecolor("#333")
        ax.grid(True, color="#2a2a2a", linewidth=0.7, linestyle="--")
        if title:
            ax.set_title(title, color=cor_txt, fontsize=11, pad=8, fontweight="bold")

    gs = fig.add_gridspec(4, 2, hspace=0.55, wspace=0.35,
                          left=0.07, right=0.97, top=0.94, bottom=0.04)

    # Cabeçalho
    ax_tit = fig.add_subplot(gs[0, :])
    ax_tit.axis("off")
    ax_tit.set_facecolor(cor_bg)
    ax_tit.text(0.5, 0.72, "MASP — Análise Estrutural Interativa",
                ha="center", va="center", fontsize=17, color=cor_est,
                fontweight="bold", fontfamily="serif", transform=ax_tit.transAxes)
    ax_tit.text(0.5, 0.25,
                f"Pórtico Plano Isostático  |  L = {L:.0f} m  |  h = {h:.1f} m  |  "
                f"q_total = {q_ef+q_viga:.1f} kN/m  |  P = {P_pont:.0f} kN @ x={x_pont:.1f} m",
                ha="center", va="center", fontsize=10, color=cor_mut,
                transform=ax_tit.transAxes)

    # Esquema estrutural
    ax1 = fig.add_subplot(gs[1, :])
    ax1.set_facecolor("#1a1a1a")
    ax1.set_title("Esquema Estrutural e Cargas", color=cor_txt, fontsize=11,
                  pad=8, fontweight="bold")

    A = (0, 0); B = (L, 0); C = (0, h); D = (L, h)
    ax1.plot([A[0],C[0]], [A[1],C[1]], "-", color=cor_est, lw=3)
    ax1.plot([B[0],D[0]], [B[1],D[1]], "-", color=cor_est, lw=3)
    ax1.plot([C[0],D[0]], [C[1],D[1]], "-", color=cor_est, lw=5)
    y_laje = h - 4.3
    ax1.plot([2, L-2], [y_laje, y_laje], "-", color="#90caf9", lw=2.5)
    ax1.text(L/2, y_laje+0.8, "Laje pendurada", ha="center",
             color="#90caf9", fontsize=8)

    for px, py in [(A[0],A[1]), (B[0],B[1])]:
        tri = patches.Polygon([[px,py],[px-1.5,py-2.5],[px+1.5,py-2.5]],
                               closed=True, facecolor="#90ee90",
                               edgecolor="darkgreen", lw=1.5)
        ax1.add_patch(tri)
        ax1.plot([px-2,px+2],[py-2.5,py-2.5],"k-",lw=1.5)

    for i in range(13):
        xi = i * L / 12
        ax1.annotate("", xy=(xi, h), xytext=(xi, h+2.5),
                     arrowprops=dict(arrowstyle="->,head_width=0.4,head_length=0.8",
                                     facecolor="#ef9a9a", lw=1.2))
    ax1.text(L/2, h+3.2, f"q = {q_ef+q_viga:.1f} kN/m",
             ha="center", color="#ef9a9a", fontsize=9, fontweight="bold")

    if P_pont > 0:
        ax1.annotate("", xy=(x_pont, h), xytext=(x_pont, h+4.8),
                     arrowprops=dict(arrowstyle="->,head_width=0.9,head_length=1.2",
                                     facecolor="#ffca28", lw=2.5))
        ax1.text(x_pont, h+5.4, f"P={P_pont:.0f} kN", ha="center",
                 color="#ffca28", fontsize=9, fontweight="bold")

    for ponto, nome, dx, dy in [(A,"A",-3,-1.5),(B,"B",1.5,-1.5),
                                 (C,"C",-3.5,0),(D,"D",1.5,0)]:
        ax1.plot(ponto[0], ponto[1], "o", color=cor_est, ms=6)
        ax1.text(ponto[0]+dx, ponto[1]+dy, nome, color=cor_txt, fontsize=10)

    ax1.annotate("", xy=(0,-3.5), xytext=(L,-3.5),
                 arrowprops=dict(arrowstyle="<->", color=cor_mut, lw=1))
    ax1.text(L/2, -4.8, f"{L:.0f} m", ha="center", color=cor_mut, fontsize=9)
    ax1.annotate("", xy=(-5,0), xytext=(-5,h),
                 arrowprops=dict(arrowstyle="<->", color=cor_mut, lw=1))
    ax1.text(-7.5, h/2, f"{h:.1f} m", ha="center", color=cor_mut,
             fontsize=9, rotation=90)

    ax1.set_xlim(-10, L+10); ax1.set_ylim(-7, h+8)
    ax1.set_aspect("equal"); ax1.axis("off")
    ax1.grid(False)

    # Diagrama V(x)
    ax2 = fig.add_subplot(gs[2, 0])
    axstyle(ax2, "Diagrama de Esforço Cortante  V(x)")
    xs, Vnum = res["xs"], res["Vnum"]
    ax2.fill_between(xs, Vnum, 0, where=Vnum>=0, color=cor_V, alpha=0.3)
    ax2.fill_between(xs, Vnum, 0, where=Vnum<0, color=cor_rup, alpha=0.3)
    ax2.plot(xs, Vnum, color=cor_V, lw=2)
    ax2.axhline(0, color=cor_mut, lw=0.8)
    ax2.set_xlabel("x [m]", color=cor_mut, fontsize=9)
    ax2.set_ylabel("V [kN]", color=cor_mut, fontsize=9)
    ax2.text(0.97, 0.95, f"|Vmax| = {res['V_max']:.1f} kN",
             transform=ax2.transAxes, ha="right", va="top",
             color=cor_V, fontsize=9, fontweight="bold")

    # Diagrama M(x)
    ax3 = fig.add_subplot(gs[2, 1])
    axstyle(ax3, "Diagrama de Momento Fletor  M(x)")
    Mnum = res["Mnum"]
    ax3.fill_between(xs, Mnum, 0, color=cor_M, alpha=0.3)
    ax3.plot(xs, Mnum, color=cor_M, lw=2)
    ax3.axhline(0, color=cor_mut, lw=0.8)
    ax3.set_xlabel("x [m]", color=cor_mut, fontsize=9)
    ax3.set_ylabel("M [kN·m]", color=cor_mut, fontsize=9)
    ax3.text(0.97, 0.95,
             f"Mmax = {res['M_max']:,.0f} kN·m\n@ x={res['x_Mmax']:.1f} m",
             transform=ax3.transAxes, ha="right", va="top",
             color=cor_M, fontsize=9, fontweight="bold")

    # Indicadores ELU
    ax4 = fig.add_subplot(gs[3, :])
    axstyle(ax4, "Indicadores de Segurança — Verificação ELU Simplificada (C35)")
    labels  = ["Utiliz. Momento  (Mr)", "Utiliz. Cortante  (Vr)", "Utiliz. Normal  (Nr)"]
    valores = [cap["uso_M"], cap["uso_V"], cap["uso_N"]]
    cores_b = [cor_rup if v > 100 else (cor_ok if v < 70 else "#ffca28")
               for v in valores]

    bars = ax4.barh(labels, valores, color=cores_b, height=0.4, edgecolor="#444")
    ax4.axvline(100, color=cor_rup, lw=1.5, ls="--", label="Limite (100%)")
    ax4.axvline(70,  color="#ffca28", lw=1.0, ls=":",  label="Atenção (70%)")
    ax4.set_xlim(0, max(max(valores)*1.25, 115))
    ax4.set_xlabel("Taxa de utilização [%]", color=cor_mut, fontsize=9)
    ax4.tick_params(axis="y", colors=cor_txt, labelsize=10)
    for bar, val in zip(bars, valores):
        ax4.text(bar.get_width()+1.5, bar.get_y()+bar.get_height()/2,
                 f"{val:.1f}%", va="center", color=cor_txt,
                 fontsize=9, fontweight="bold")
    ax4.legend(loc="lower right", labelcolor=cor_txt, fontsize=8,
               facecolor="#1a1a1a", edgecolor="#333")

    info = (f"Reações:  RA = {res['R_Av']:.1f} kN  |  RB = {res['R_Bv']:.1f} kN  |  "
            f"N_pil_esq = {res['N_pil_esq']:.1f} kN  |  N_pil_dir = {res['N_pil_dir']:.1f} kN  |  "
            f"N_viga (protensão) = {res['N_viga']:.1f} kN")
    fig.text(0.5, 0.005, info, ha="center", color=cor_mut, fontsize=8.5)
    return fig


# ─── Interface ───────────────────────────────
st.set_page_config(page_title="MASP — Análise Estrutural", layout="wide",
                   page_icon="🏛️")

st.markdown("""
<style>
body, .stApp { background: #0f0f0f; color: #f0f0f0; }
h1, h2, h3 { font-family: Georgia, serif; color: #e0c87a; }
.stSidebar { background: #141414; }
</style>
""", unsafe_allow_html=True)

st.title("🏛️ MASP — Análise Estrutural Interativa")
st.markdown("""
*Pórtico plano isostático simplificado · Projeto de **Lina Bo Bardi** (1968)*  
Vão livre suspenso de **70 m** suportado por vigas-caixão protendidas e quatro pilares.
""")

# Sidebar
st.sidebar.header("⚙️ Parâmetros de Carga")
q_ef = st.sidebar.number_input(
    "Carga distribuída — viga superior EF [kN/m]",
    value=132.0, step=5.0, min_value=0.0,
    help="132 kN/m (estudo de caso real)"
)
q_viga = st.sidebar.number_input(
    "Carga distribuída — laje pendurada CD [kN/m]",
    value=327.0, step=10.0, min_value=0.0,
    help="327 kN/m (estudo de caso real)"
)
N_pret = st.sidebar.number_input(
    "Força de protensão axial [kN]",
    value=100_000.0, step=5_000.0, min_value=0.0,
    help="Compressão axial na viga-caixão protendida"
)
st.sidebar.markdown("---")
P_pont = st.sidebar.number_input(
    "Carga pontual P [kN]", value=0.0, step=500.0, min_value=0.0,
    help="Carga extra — ex.: equipamento ou sobrecarga concentrada"
)
x_pont = st.sidebar.slider(
    "Posição de P na viga [m]", 0.0, 70.0, 35.0, 0.5,
    help="0 m = extremidade esquerda (C), 70 m = extremidade direita (D)"
)
st.sidebar.markdown("---")
L = st.sidebar.number_input("Vão livre L [m]", value=70.0, step=1.0, min_value=10.0)
h = st.sidebar.number_input("Altura dos pilares h [m]", value=11.0, step=0.5, min_value=3.0)

x_pont = min(x_pont, float(L))

# Cálculo
res = resolver_portico(q_ef, q_viga, N_pret, P_pont, x_pont, L, h)
cap = capacidade_ruptura(res)

# Métricas de topo
col_a, col_b, col_c, col_d = st.columns(4)
col_a.metric("RA", f"{res['R_Av']:,.0f} kN")
col_b.metric("RB", f"{res['R_Bv']:,.0f} kN")
col_c.metric("Mmax", f"{res['M_max']:,.0f} kN·m")
col_d.metric("Vmax", f"{res['V_max']:,.0f} kN")

st.markdown("---")

# Badges ELU
c1, c2, c3 = st.columns(3)
for col, titulo, val in [(c1,"Utiliz. Momento",cap["uso_M"]),
                          (c2,"Utiliz. Cortante",cap["uso_V"]),
                          (c3,"Utiliz. Normal",cap["uso_N"])]:
    emoji = "🔴" if val>100 else ("🟡" if val>70 else "🟢")
    col.metric(f"{emoji} {titulo}", f"{val:.1f} %")

# Plot
with st.spinner("Calculando..."):
    fig = plot_masp(res, cap, q_ef, q_viga, N_pret, P_pont, x_pont, L, h)
    st.pyplot(fig)

# Download PDF
pdf_bytes = io.BytesIO()
fig.savefig(pdf_bytes, format="pdf", facecolor="#0f0f0f")
pdf_bytes.seek(0)
st.download_button("📄 Baixar PDF da Análise", data=pdf_bytes,
                   file_name="masp_analise_estrutural.pdf",
                   mime="application/pdf")

# Info técnica
st.markdown("---")
st.markdown("## 📐 Sobre a Estrutura do MASP")
col1, col2 = st.columns(2)
with col1:
    st.markdown("""
### Modelo adotado
O **pórtico plano isostático** representa a seção transversal do MASP com:

| Elemento | Descrição |
|---|---|
| Apoios A e B | Articulados — sapatas excêntricas sobre aterro |
| Pilares AC e BD | Seções maciças de CA, h ≈ 11 m |
| Viga CD | Viga-caixão protendida, vão livre 70 m |
| Laje pendurada | Suspensa por tirantes à viga principal |
| Protensão axial | 100.000 kN (compressão) |

As cargas de **132 kN/m** (viga superior) e **327 kN/m** (laje pendurada)  
correspondem ao estudo de caso do pórtico isostático.
""")
with col2:
    st.markdown("""
### Verificação ELU simplificada
| Parâmetro | Valor |
|---|---|
| Concreto | C35 — fck = 35 MPa |
| fcd = fck / γc | ≈ 23,3 MPa |
| Aço | CA-50 — fyd = 434 MPa |
| Seção da viga (b × d) | 1,5 × 3,0 m |
| Mr (momento resistente) | calculado pela expressão simplificada |
| Vr (cortante resistente) | τd × b × d |

> ⚠️ Verificação **didática e linearizada**.  
> Uma análise real exige modelagem em MEF conforme NBR 6118.

### Grau de hiperestasia
O modelo usa o pórtico **isostático** (Simulação 1 do estudo de caso).  
A versão hiperestática (Simulação 2) é resolvida pelo *Método dos Esforços*.
""")

st.markdown("---")
st.caption("Estudo de Caso MASP — Pórtico Plano Isostático | Mecânica das Estruturas · POLI-USP")

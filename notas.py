import numpy as np
import matplotlib.pyplot as plt

# Valores das provas e pesos
peso_p1 = 2
peso_p2 = 2.5
peso_p3 = 3.5
peso_testinhos = 2

# Notas obtidas
nota_p1 = 2.4
nota_testinhos = 4

# Meta de média
media_desejada = 4.95

# Soma total dos pesos
soma_pesos = peso_p1 + peso_p2 + peso_p3 + peso_testinhos

# Cálculo da nota ponderada já obtida
nota_ponderada_atual = (nota_p1 * peso_p1) + (nota_testinhos * peso_testinhos)

# Nota ponderada necessária total para média 5
nota_ponderada_necessaria = media_desejada * soma_pesos

# Nota ponderada que ainda precisa ser obtida em P2 e P3
nota_ponderada_restante = nota_ponderada_necessaria - nota_ponderada_atual

print(f"Nota ponderada atual: {nota_ponderada_atual}")
print(f"Nota ponderada necessária total: {nota_ponderada_necessaria}")
print(f"Nota ponderada restante (P2 + P3): {nota_ponderada_restante}")

# Equação: peso_p2 * p2 + peso_p3 * p3 = nota_ponderada_restante
# Isolando p3: p3 = (nota_ponderada_restante - peso_p2 * p2) / peso_p3

# Criando range de notas para P2 (0 a 10)
p2_values = np.linspace(0, 10, 1000)

# Calculando P3 correspondente para cada valor de P2
p3_values = (nota_ponderada_restante - peso_p2 * p2_values) / peso_p3

# Criando o gráfico
plt.figure(figsize=(12, 8))

# Plotando a linha principal
plt.plot(p2_values, p3_values, "b-", linewidth=2, label=f"Média = {media_desejada}")

# Limitando aos valores válidos (0 a 10)
valid_mask = (p3_values >= 0) & (p3_values <= 10)
plt.fill_between(
    p2_values[valid_mask],
    0,
    p3_values[valid_mask],
    alpha=0.2,
    color="green",
    label="Região viável",
)

# Configurações do gráfico
plt.xlim(0, 10)
plt.ylim(0, 10)
plt.xlabel("Nota P2", fontsize=12, fontweight="bold")
plt.ylabel("Nota P3", fontsize=12, fontweight="bold")
plt.title(
    f"Combinações de P2 e P3 para Média {media_desejada}\n(P1={nota_p1}, Testinhos={nota_testinhos})",
    fontsize=14,
    fontweight="bold",
)

# Grid detalhado
plt.grid(True, linestyle="-", alpha=0.3)
plt.minorticks_on()
plt.grid(True, which="minor", linestyle=":", alpha=0.2)

# Marcadores nos eixos a cada 0.5 pontos
major_ticks = np.arange(0, 11, 1)
minor_ticks = np.arange(0, 10.5, 0.5)
plt.xticks(major_ticks)
plt.yticks(major_ticks)
plt.gca().set_xticks(minor_ticks, minor=True)
plt.gca().set_yticks(minor_ticks, minor=True)

# Pontos específicos de interesse
pontos_interesse = [
    (5, (nota_ponderada_restante - peso_p2 * 5) / peso_p3),
    (6, (nota_ponderada_restante - peso_p2 * 6) / peso_p3),
    (6.5, (nota_ponderada_restante - peso_p2 * 6.5) / peso_p3),
    (7, (nota_ponderada_restante - peso_p2 * 7) / peso_p3),
    (7.5, (nota_ponderada_restante - peso_p2 * 7.5) / peso_p3),
    (8, (nota_ponderada_restante - peso_p2 * 8) / peso_p3),
    (8.5, (nota_ponderada_restante - peso_p2 * 8.5) / peso_p3),
    (9, (nota_ponderada_restante - peso_p2 * 9) / peso_p3),
]

for p2, p3 in pontos_interesse:
    if 0 <= p3 <= 10:
        plt.plot(p2, p3, "ro", markersize=8)
        plt.annotate(
            f"P2={p2:.1f}\nP3={p3:.1f}",
            xy=(p2, p3),
            xytext=(10, 10),
            textcoords="offset points",
            bbox=dict(boxstyle="round,pad=0.3", facecolor="yellow", alpha=0.7),
            fontsize=9,
            ha="center",
        )

# Linha para nota mínima (caso queira ver onde fica impossível)
plt.axhline(y=0, color="red", linestyle="--", alpha=0.5, label="Limite inferior")
plt.axhline(y=10, color="red", linestyle="--", alpha=0.5, label="Limite superior")
plt.axvline(x=0, color="red", linestyle="--", alpha=0.5)
plt.axvline(x=10, color="red", linestyle="--", alpha=0.5)

plt.legend(loc="upper right")

# Adicionando texto explicativo
textstr = f"""Equação: {peso_p2:.1f} × P2 + {peso_p3:.1f} × P3 = {nota_ponderada_restante:.1f}
Nota atual: {nota_ponderada_atual:.1f}/{nota_ponderada_necessaria:.1f}"""
props = dict(boxstyle="round", facecolor="lightblue", alpha=0.8)
plt.text(0.5, 9.5, textstr, fontsize=10, bbox=props)

plt.tight_layout()
plt.show()

# Tabela com algumas combinações exatas
print("\n" + "=" * 50)
print("COMBINAÇÕES PARA MÉDIA 4.95:")
print("=" * 50)
print(f"{'P2':<6} {'P3':<6} {'Média':<8}")
print("-" * 20)

for p2 in range(0, 11):
    p3 = (nota_ponderada_restante - peso_p2 * p2) / peso_p3
    if 0 <= p3 <= 10:
        media_calc = (
            nota_p1 * peso_p1
            + p2 * peso_p2
            + p3 * peso_p3
            + nota_testinhos * peso_testinhos
        ) / soma_pesos
        print(f"{p2:<6} {p3:<6.2f} {media_calc:<8.2f}")

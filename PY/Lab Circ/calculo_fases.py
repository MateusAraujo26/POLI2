import numpy as np

# Dados Medidos (1 kHz)
VE = 3.4433
VB = 2.3419
VR = 2.1539

print("=== DADOS MEDIDOS ===")
print(f"VE = {VE} V")
print(f"VB = {VB} V")
print(f"VR = {VR} V")
print()

# Cálculo das fases (em radianos e depois graus)
print("=== CÁLCULO DE THETA_B ===")
print("Fórmula: theta_B = arccos((VE² - VR² - VB²) / (2 * VR * VB))")
numerador_B = VE**2 - VR**2 - VB**2
denominador_B = 2 * VR * VB
print(f"VE² = {VE**2:.4f}")
print(f"VR² = {VR**2:.4f}")
print(f"VB² = {VB**2:.4f}")
print(f"Numerador: VE² - VR² - VB² = {numerador_B:.4f}")
print(f"Denominador: 2 * VR * VB = {denominador_B:.4f}")
argumento_B = numerador_B / denominador_B
print(f"Argumento do arccos: {argumento_B:.4f}")
theta_B = np.arccos(argumento_B)
print(f"theta_B = {theta_B:.4f} rad")
theta_B_deg = np.degrees(theta_B)
print(f"theta_B = {theta_B_deg:.2f}°")
print()

print("=== CÁLCULO DE THETA_E ===")
print("Fórmula: theta_E = arccos((VE² + VR² - VB²) / (2 * VR * VE))")
numerador_E = VE**2 + VR**2 - VB**2
denominador_E = 2 * VR * VE
print(f"VE² = {VE**2:.4f}")
print(f"VR² = {VR**2:.4f}")
print(f"VB² = {VB**2:.4f}")
print(f"Numerador: VE² + VR² - VB² = {numerador_E:.4f}")
print(f"Denominador: 2 * VR * VE = {denominador_E:.4f}")
argumento_E = numerador_E / denominador_E
print(f"Argumento do arccos: {argumento_E:.4f}")
theta_E = np.arccos(argumento_E)
print(f"theta_E = {theta_E:.4f} rad")
theta_E_deg = np.degrees(theta_E)
print(f"theta_E = {theta_E_deg:.2f}°")
print()

# --- Sanity Check ---
print("=== SANITY CHECK ===")
# Reconstrói os fasores para verificar se VR + VB = VE
# Referência: VR está em 0 graus
V_R_fasor = complex(VR, 0)
V_B_fasor = complex(VB * np.cos(theta_B), VB * np.sin(theta_B))
V_E_fasor_soma = V_R_fasor + V_B_fasor

print(f"Theta_B: {theta_B_deg:.2f}°")
print(f"Theta_E: {theta_E_deg:.2f}°")
print(f"Check Módulo VE: {abs(V_E_fasor_soma):.4f} V (Esperado: {VE})")
print(
    f"Check Fase VE: {np.degrees(np.angle(V_E_fasor_soma)):.2f}° (Esperado: {theta_E_deg:.2f}°)"
)

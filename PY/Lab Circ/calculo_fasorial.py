import numpy as np

def to_cartesian(magnitude, phase_deg):
    """Converte magnitude e fase (graus) para número complexo."""
    phase_rad = np.deg2rad(phase_deg)
    real = magnitude * np.cos(phase_rad)
    imag = magnitude * np.sin(phase_rad)
    return complex(real, imag)

# Dados experimentais fornecidos
medicoes = {
    100: {
        'VE': {'mag': 5.0,  'phase': 0},      # VE é a nossa referência (0°)
        'VR': {'mag': 4.1,  'phase': -6.0},   # Atrasado em relação a VE
        'VB': {'mag': 1.41, 'phase': 20.0}    # Adiantado em relação a VE
    },
    1000: {
        'VE': {'mag': 4.98, 'phase': 0},
        'VR': {'mag': 3.06, 'phase': -43.65},
        'VB': {'mag': 3.53, 'phase': 37.5}
    }
}

print(f"{'Freq (Hz)':<10} | {'Fasor':<5} | {'Forma Polar':<18} | {'Forma Cartesiana (V)'}")
print("-" * 70)

for freq, comp in medicoes.items():
    for nome, val in comp.items():
        z = to_cartesian(val['mag'], val['phase'])
        polar = f"{val['mag']:.2f} ∠ {val['phase']:.2f}°"
        cartesian = f"{z.real:.3f} + j({z.imag:.3f})"
        print(f"{freq:<10} | {nome:<5} | {polar:<18} | {cartesian}")
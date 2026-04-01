@ /000

MAIN      LD PTR
          MM P_CURR
          LD COUNT
          MM C_CURR
          
LOOP      LD C_CURR
          JZ START_PR
          
          GD /000
          ML K_0100
          MM RN_TEMP
          GD /000
          AD RN_TEMP
          MM RN_RES
          
          LD BASE_MM
          AD P_CURR
          MM STORE_OP
          
          LD RN_RES
STORE_OP  K /0000
          
          LD P_CURR
          AD K_02
          MM P_CURR
          
          LD C_CURR
          SB K_01
          MM C_CURR
          
          LD C_CURR
          JZ START_PR
          GD /000     ; Consome S1
          GD /000     ; Consome S2
          JP LOOP
          
START_PR  LD COUNT
          MM C_CURR
          LD P_CURR
          SB K_02
          MM P_CURR
          
PR_LOOP   LD C_CURR
          JZ FIM
          
          LD BASE_LD
          AD P_CURR
          MM LOAD_OP
          
LOAD_OP   K /0000
          MM VAL_P
          
          LD VAL_P
          DV K_0100
          PD /001
          
          LD VAL_P
          MM TEMP
          DV K_0100
          ML K_0100
          MM MASK
          LD TEMP
          SB MASK
          PD /001
          
          LD P_CURR
          SB K_02
          MM P_CURR
          
          LD C_CURR
          SB K_01
          MM C_CURR
          JP PR_LOOP
          
FIM       HM /000

; Variaveis e Constantes
@ /100
BASE_MM   K /9000
@ /102
BASE_LD   K /8000
@ /104
PTR       K /0300
@ /106
P_CURR    K /0000
@ /108
COUNT     K /000A
@ /10A
C_CURR    K /0000
@ /10C
K_01      K /0001
@ /10E
K_02      K /0002
@ /110
RN_TEMP   K /0000
@ /112
RN_RES    K /0000
@ /114
K_0100    K /0100
@ /116
VAL_P     K /0000
@ /118
TEMP      K /0000
@ /11A
MASK      K /0000

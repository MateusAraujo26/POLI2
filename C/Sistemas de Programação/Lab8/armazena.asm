@ /000

MAIN      LD PTR
          MM P_CURR
          LD COUNT
          MM C_CURR

LOOP      LD C_CURR
          JZ FIM
          
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
          JZ SKIP_S
          GD /000     ; Consome S1
          GD /000     ; Consome S2
SKIP_S    JP LOOP
          
FIM       HM /000

; Constantes e Variaveis
@ /100
BASE_MM   K /9000
@ /102
PTR       K /0300
@ /104
P_CURR    K /0000
@ /106
COUNT     K /000A
@ /108
C_CURR    K /0000
@ /10A
K_01      K /0001
@ /10C
K_02      K /0002
@ /10E
RN_TEMP   K /0000
@ /110
RN_RES    K /0000
@ /112
K_0100    K /0100

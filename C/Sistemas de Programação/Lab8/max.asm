@ /000

MAIN    SC READ_VAL
        MM MAX
        LD COUNT
        SB ONE
        MM COUNT

LOOP    LD COUNT
        JZ END_PRG
        SC READ_VAL
        MM TEMP
        LD TEMP
        SB MAX
        JN CONTINUE
        LD TEMP
        MM MAX

CONTINUE  LD COUNT
          SB ONE
          MM COUNT
          JP LOOP

END_PRG   LD MAX
          MM TEMP_P
          DV K16
          PD /001
          LD TEMP_P
          MM TEMP2
          DV K16
          ML K16
          MM TEMP3
          LD TEMP2
          SB TEMP3
          PD /001
          HM /000

READ_VAL  K /0000
          GD /000
          ML K16
          MM RD_T
          GD /000
          AD RD_T
          MM RD_RES
          
          ; Teste se lemos 2 caracteres ou nao dependendo se acabou!
          LD COUNT
          SB ONE
          JZ SKIP_S
          GD /000
          GD /000
SKIP_S    LD RD_RES
          RS READ_VAL

; Variaveis e Constantes
@ /100
MAX      K /0000

@ /102
TEMP     K /0000

@ /104
COUNT    K /0005

@ /106
ONE      K /0001

@ /108
K16      K /0100

@ /10A
TEMP_P   K /0000

@ /10C
TEMP2    K /0000

@ /10E
TEMP3    K /0000

@ /110
RD_T     K /0000

@ /112
RD_RES   K /0000

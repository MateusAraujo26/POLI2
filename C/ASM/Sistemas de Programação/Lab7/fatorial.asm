@ /000

MAIN   SC FATORIAL
       HM /000       ; Para a execução da MVN

; Constantes
CONST_1   K /0001
RETORNO   K /0000      ; Endereço de retorno

FATORIAL  LD N          ; Carrega o valor do endereço do operando em AC: 
                        ; Carrega N.
          JZ CASE_BASE  ; Desvio para o endereço do operando se AC é zero: 
                        ; Se N=0, pula para caso base.
          MM CONTADOR   ; Carrega no endereço do operando o valor de AC:
                        ; Salva N no contador
          LD CONST_1       
          MM RES        ; Inicializa resultado com 1

LOOP      LD RES            ; Resultado parcial
          ML CONTADOR       ; Multiplica pelo contador
          MM RES            ; Salva novo resultado parcial
          LD CONTADOR       ; Carrega contador
          SB CONST_1        ; Decrementa contador
          JZ FIM_FATORIAL   ; Se contador = 0, fim
          MM CONTADOR       ; Salva novo valor do contador
          JP LOOP           ; Desvio incondicional para o endereço do operando:
                            ; Próxima iteração

CASE_BASE   LD CONST_1      ; Se N = 0, carrega 1
            MM RES          ; Resultado = 1

FIM_FATORIAL  RS FATORIAL   ; Return

; Variáveis
@ /100
N         K /0005
@ /102
RES       K /0000         ; Resultado final
@ /104
CONTADOR  K /0000         ; Variável de loop
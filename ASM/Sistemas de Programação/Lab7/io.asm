@ /000

MAIN       SC READ_NUM     ; Lê 2 dígitos e monta palavra ASCII em AC
           MM VAL_X        ; Salva X

           SC READ_NUM     ; Lê 2 dígitos e monta palavra ASCII em AC
           MM VAL_Y        ; Salva Y

           ; Subtrair 0x3030
           LD VAL_X
           SB K_3030       ; X - 0x3030
           MM AUX_X

           LD VAL_Y
           SB K_3030       ; Y - 0x3030
           MM AUX_Y

           ; Somar os números
           LD AUX_X
           AD AUX_Y        
           MM SOMA         ; Resultado parcial

           ; Verificar carry do menos significativo
           LD SOMA
           MM SOMA_TEMP
           DV K_0010       ; Divisão de int por 16 
           ML K_0010       ; Multiplica por 16
           MM MASK_TEMP    
           LD SOMA_TEMP
           SB MASK_TEMP    ; Restou o menos significativo
           
           SB K_000A       ; Subtrai 10
           JN NO_CARRY     ; Se deu negativo, não tem carry

           ; Caso tenha carry
           LD SOMA
           SB K_000A       ; Subtrai 10 do menos significativo
           AD K_0100       ; Soma 1 no segundo menos significativo
           MM SOMA         ; Atualiza soma

NO_CARRY   LD SOMA
           AD K_3030       ; Restaura ASCII
           MM RESULTADO 

           ; Imprimir
           LD RESULTADO
           DV K_0100       ; Pega byte alto
           PD /1           ; Imprime caractere

           ; Imprimir menos significativo
           LD RESULTADO    ; 0x3631
           MM SOMA_TEMP
           DV K_0100       ; 0x0036
           ML K_0100       ; 0x3600
           MM MASK_TEMP
           LD SOMA_TEMP    ; 0x3631
           SB MASK_TEMP    ; 0x0031 (1)
           PD /1           ; Imprime caractere

           HM /000          ; Fim

; Lê dois dígitos hexa e retorna no formato ASCII.
READ_NUM  K /0000
           ; Lê primeiro dígito
           GD /0           
           AD K_0030       ; Converte para ASCII 
           ML K_0100       ; Move para byte alto
           MM RN_TEMP

           ; Lê segundo dígito
           GD /0           
           AD K_0030       ; Converte para ASCII 
           
           AD RN_TEMP      ; Junta com byte alto
           RS READ_NUM     ; Retorna com 0x3037


; Constantes e Variáveis
K_000A    K /000A         
K_0010    K /0010         
K_0030    K /0030         ; Offset ASCII unitário
K_0100    K /0100         ; Offset Carry
K_3030    K /3030         ; Máscara ASCII dupla

VAL_X     K /0000
VAL_Y     K /0000
AUX_X     K /0000
AUX_Y     K /0000
SOMA      K /0000
RESULTADO K /0000

; Variáveis temporárias
RN_TEMP   K /0000
SOMA_TEMP K /0000
MASK_TEMP K /0000
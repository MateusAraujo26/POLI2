@ /000

MAIN       LD BASE_MM
LOOP       LD BASE_MM      
           AD PTR         
           MM STORE_INSTR  ; Grava a nova instrução no STORE_INSTR

           LD SQUARE       ; Carrega quadrado calculado
STORE_INSTR K /0000       
           ; (n+1)^2 = n^2 + (2n + 1)
           AD ODD_TERM     ; AC já tinha SQUARE, soma ODD_TERM
           MM SQUARE       ; Atualiza SQUARE
           
           ; Atualiza o termo ímpar
           LD ODD_TERM
           AD CONST_2      ; Próximo ímpar = atual + 2
           MM ODD_TERM

           ; Atualizar ponteiro de memória (+2)
           LD PTR
           AD CONST_2
           MM PTR

           ; Decrementar contador de iterações
           LD COUNTER      ; Carrega 64
           SB CONST_1      
           MM COUNTER      
           JZ FIM          ; Se chegou a 0, termina
           JP LOOP         

FIM        HM /000          

; Variáveis
BASE_MM    K /9000         ; Base para instrução MM
PTR        K /0100         ; Ponteiro
SQUARE     K /0000         ; Acumulador do quadrado
ODD_TERM   K /0001         ; Termo da somatória
COUNTER    K /0040         ; Contador de 64 iterações
CONST_1    K /0001
CONST_2    K /0002
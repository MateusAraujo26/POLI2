/*
Crie um programa chamado load (arquivo load.c), que lê uma sequência de valores hexadecimais e armazena em memória. 
Os valores hexadecimais devem ser compostos de quatro dígitos, com os caracteres alfabéticos em maiúsculo. 
O programa recebe primeiro a quantidade de valores que serão digitados e, em seguida, recebe cada valor, digitados um por vez. 
A memória é um arquivo chamado mem.out. Os valores devem ser armazenados sequencialmente em uma posição de memória, 
sendo que cada posição é um número hexadecimal de quatro dígitos. Toda vez que o programa load for chamado, 
ele deve apagar o conteúdo do arquivo mem.out, caso este arquivo já exista.

O arquivo mem.out começa com o endereço de memória, seguido de um espaço e o valor a ser armazenado. 
O próximo valor deve ser armazenado na linha seguinte e o endereço seguinte de memória deve ser incrementado em 2 (0000, 0002, 0004, ...). 
Veja um exemplo do conteúdo de memória a seguir:

0000 011F
0002 0101
0004 A2AD
0006 F125
0008 8554
000A 6D6C
...

Ao executar o programa, é exibida a mensagem "Quantidade de valores:". 
Após terminar, a seguinte mensagem é exibida: "Valores salvos na memoria." As mensagens devem ser seguidas por uma quebra de linha. 
A chamada do programa fica assim:

$ ./load
Quantidade de valores:
$ 4
$ 0001
$ 0300
$ 80EA
$ 1DC0
Valores salvos na memoria.
*/
/*
Crie um programa chamado load (arquivo load.c), que le uma sequencia de valores hexadecimais e armazena em memoria. 
Os valores hexadecimais devem ser compostos de quatro digitos, com os caracteres alfabeticos em maiusculo. 
O programa recebe primeiro a quantidade de valores que serao digitados e, em seguida, recebe cada valor, digitados um por vez. 
A memoria e um arquivo chamado mem.out. Os valores devem ser armazenados sequencialmente em uma posicao de memoria, 
sendo que cada posicao e um numero hexadecimal de quatro digitos. Toda vez que o programa load for chamado, 
ele deve apagar o conteudo do arquivo mem.out, caso este arquivo ja exista.

O arquivo mem.out comeca com o endereco de memoria, seguido de um espaco e o valor a ser armazenado. 
O proximo valor deve ser armazenado na linha seguinte e o endereco seguinte de memoria deve ser incrementado em 2 (0000, 0002, 0004, ...). 
Veja um exemplo do conteudo de memoria a seguir:

0000 011F
0002 0101
0004 A2AD
0006 F125
0008 8554
000A 6D6C
...

Ao executar o programa, e exibida a mensagem "Quantidade de valores:". 
Apos terminar, a seguinte mensagem e exibida: "Valores salvos na memoria." As mensagens devem ser seguidas por uma quebra de linha. 
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

#include <stdio.h>

int main() {
    FILE *mem;
    int quantidade;

    mem = fopen("mem.out", "w"); //apaga com w se tiver algo

    printf("Quantidade de valores:\n");
    scanf("%d", &quantidade);

    if (quantidade > 20) {
        quantidade = 20;
    }

    char valores[20][5]; //20 valores, cada um com 4 digitos + \0

    for (int i = 0; i < quantidade; i++){
        scanf("%s", valores[i]);
        fprintf(mem, "%04X %s\n", i*2, valores[i]); //guarda no endereco de memoria
    }

    printf("Valores salvos na memoria.\n");

    fclose(mem);
    return 0;
}
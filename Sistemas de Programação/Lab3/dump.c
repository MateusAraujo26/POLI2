/*
Crie um programa chamado dump (dump.c) que imprime os valores armazenados na memoria (mem.out), 
seguindo o mesmo formato do arquivo, incluindo o endereco correspondente. 
Por exemplo:

$ ./dump
0000 0520
0002 06F0
0004 1EED
...
*/

#include <stdio.h>

int main() {
    FILE *mem;
    char endereco[5];
    char valor[5];

    mem = fopen("mem.out", "r");

    while(fscanf(mem, "%s %s\n", endereco, valor) == 2) //fscanf retorna int
        printf("%s %s\n", endereco, valor);

    fclose(mem);
    return 0;
}
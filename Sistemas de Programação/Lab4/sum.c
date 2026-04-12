/*
Crie o programa chamado sum (sum.c), que le dois enderecos de memoria, passados como parametro, um por vez, 
e devolve o resultado da soma em hexadecimal com quatro digitos (ou cinco caso o resultado da soma seja maior).  

A chamada do programa fica assim:

$ ./sum
$ 0002
$ 00EF
00F1
*/

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        return 1;
    }

    FILE *mem = fopen("mem.out", "r");
    if (!mem) {
        return 1;
    }

    int endereco_busca1, endereco_busca2;
    sscanf(argv[1], "%X", &endereco_busca1);
    sscanf(argv[2], "%X", &endereco_busca2);

    int valor1 = -1, valor2 = -1;
    int endereco_lido, valor_lido;

    // Busca os valores nos enderecos especificados
    while (fscanf(mem, "%X %X", &endereco_lido, &valor_lido) == 2) {
        if (endereco_lido == endereco_busca1) {
            valor1 = valor_lido;
        }
        if (endereco_lido == endereco_busca2) {
            valor2 = valor_lido;
        }
    }

    fclose(mem);

    // Soma os valores encontrados
    int resultado = valor1 + valor2;
    printf("%04X\n", resultado);

    return 0;
}
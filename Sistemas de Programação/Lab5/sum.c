/*
Modifique o programa sum (sum.c) para receber um único endereço de 4 dígitos 
na mesma linha de chamada do programa. 
O programa deve somar o valor desse endereço ao valor do acumulador 
e armazená-lo no próprio acumulador. O programa deve imprimir em tela o valor da soma. 
Um exemplo de chamada de sum:

$ ./sum 0002
*/

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    unsigned short endereco, valor_mem = 0, valor_acc = 0;

    sscanf(argv[1], "%hX", &endereco);

    FILE *acc = fopen("acc.out", "r");
    fscanf(acc, "%hX", &valor_acc);
    fclose(acc);

    FILE *mem = fopen("mem.out", "r");
    if (mem != NULL) {
        unsigned short endereco_busca, valor_busca;
        while (fscanf(mem, "%hX %hX", &endereco_busca, &valor_busca) == 2) {
            if (endereco_busca == endereco) {
                valor_mem = valor_busca;
                break;
            }
        }
        fclose(mem);
    }

    unsigned short resultado = valor_acc + valor_mem;

    FILE *acc_out = fopen("acc.out", "w");
    fprintf(acc_out, "%04X\n", resultado);
    fclose(acc_out);

    printf("%04X\n", resultado);

    return 0;
}
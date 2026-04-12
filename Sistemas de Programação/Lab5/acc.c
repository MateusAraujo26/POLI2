/*
No laboratório de hoje, vamos construir novos programas para realizar algumas operações básicas de um computador, 
além de aproveitar os programas feitos nos laboratórios anteriores. 
Vocês também vão escrever um programa simulador para realizar essas operações por meio de instruções padronizadas.
*/

/*
Exercício 1 (1,0 ponto): Crie o programa acc (acc.c), que cria e carrega um valor no acumulador (acc.out) 
a partir de um endereço de memória passado como parâmetro. O programa deve imprimir em tela o valor presente no endereço armazenado. 
Caso exista, o arquivo acc.out deve ser sobrescrito. Por exemplo:

$ ./acc 0004
*/

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    unsigned short endereco;
    unsigned short valor = 0;
    sscanf(argv[1], "%hX", &endereco);
    
    FILE *mem = fopen("mem.out", "r");

    unsigned short endereco_busca, valor_busca;
    while (fscanf(mem, "%hX %hX", &endereco_busca, &valor_busca) == 2) {
        if (endereco_busca == endereco) {
            valor = valor_busca;
            break;
        }
    }
    fclose(mem);

    FILE *acc = fopen("acc.out", "w");
    fprintf(acc, "%04X\n", valor);
    fclose(acc);
    
    printf("%04X\n", valor);

    return 0;
}
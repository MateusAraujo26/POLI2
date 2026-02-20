/*
No laboratório de hoje, vamos construir novos programas para realizar algumas operações básicas de um computador, 
além de aproveitar os programas feitos nos laboratórios anteriores. 
Vocês também vão escrever um programa simulador para realizar essas operações por meio de instruções padronizadas.
*/

/*
Exercício 2 (1,0 ponto): Crie o programa mem (mem.c), 
que carrega um valor do acumulador no endereço de memória passado como parâmetro. 
O arquivo mem.out deve ser atualizado. 
O programa deve imprimir em tela o valor que estava no endereço de memória e que foi substituído. 
Veja um exemplo de chamada:

$ ./mem 0002
*/

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    unsigned short endereco, valor_carregado, valor_anterior = 0;
    unsigned short enderecos[1000], valores[1000];
    int total = 0, encontrado = 0;

    sscanf(argv[1], "%hX", &endereco);

    FILE *acc = fopen("acc.out", "r");
    fscanf(acc, "%hX", &valor_carregado);
    fclose(acc);

    FILE *mem_buffer = fopen("mem.out", "r");
    while (fscanf(mem_buffer, "%hX %hX", &enderecos[total], &valores[total]) == 2) {
        if (enderecos[total] == endereco) {
            valor_anterior = valores[total];
            valores[total] = valor_carregado;
            encontrado = 1;
        }
        total++;
    }
    fclose(mem_buffer);

    FILE *mem_out = fopen("mem.out", "w");
    for (int i = 0; i < total; i++) {
        fprintf(mem_out, "%04X %04X\n", enderecos[i], valores[i]);
    }
    fclose(mem_out);

    printf("%04X\n", valor_anterior);

    return 0;
}
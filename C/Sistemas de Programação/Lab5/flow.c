/*
No laboratório de hoje, vamos continuar criando programas para fazer operações básicas de um computador: subtração, uso de registradores, comparações, laços e busca. Para isso, vocês vão escrever programas na linguagem C que fazem essas operações nas atividades a seguir.
Vamos utilizar também a representação complemento de 2 para trabalhar com os valores hexadecimais. Essa representação é bastante usada em sistemas digitais, incluindo a maioria dos computadores, sendo muito útil para conseguirmos trabalhar com números inteiros positivos e negativos.
Para obter a representação complemento de 2, devemos primeiro obter o complemento de 1, invertendo o número pelo seu valor complementar e, em seguida, somar 1. Além disso, o dígito mais significativo indica se o valor é positivo ou negativo. 
Quando trabalhamos com valores binários, é muito fácil obter o valor complementar: 0 vira 1 e 1 vira 0. Já com valores hexadecimais, formados por 16 possíveis valores, exigem um pouco mais de cuidado: o complemento y de um valor x cuja soma de x + y = F. Por exemplo, 0 + F = F, 1 + E = F, e 8 + 7 = F. Os valores positivos têm o bit mais significativo entre 0 e 7. Já os negativos ficam entre 8 e F. Isso porque 7 e 8 dividem o total de números que podem ser representados com essa notação, o que possibilita ter a mesma quantidade de valores positivos e negativos.


Observações: 
Todos os exercícios desta aula usam a representação complemento de 2.
Receba os parâmetros modificando a declaração da função main.
Os arquivos devem ser salvos na codificação UTF-8 (padrão do linux).
Cuidado para não gerar o arquivo mem.out com 'lixo' da memória.
Use o fopen para manipulação de arquivos.
*/

/*
Crie um programa flow (flow.c) que recebe um valor de entrada e verifica se 
a diferença entre o valor do acumulador e esse valor de entrada é positiva ou negativa 
(valor do acumulador - valor de entrada). 
Se a diferença for positiva (ou zero), armazena o valor de entrada em um novo endereço de memória 
na próxima posição de memória livre, sem apagar o conteúdo atual de mem.out.  
O programa deve imprimir o último endereço de memória ocupado. 
Um exemplo de chamada de flow é mostrado a seguir:

$ ./flow 000A
*/

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int endereco_memoria1, endereco_memoria2;

    sscanf(argv[1], "%X", &endereco_memoria1);

    FILE *acc = fopen("acc.out", "r");
    fscanf(acc, "%X", &endereco_memoria2);
    fclose(acc);

    unsigned short diferenca = (endereco_memoria2 + (~endereco_memoria1 + 1));

    if((diferenca & 0x8000) == 0){ // bit mais significativo deve ser 0
        FILE* memoria = fopen("mem.out","r");

        unsigned short ultimo_endereco = 0;
        unsigned short endereco_temp;
        char linha[20];
        while (fgets(linha, sizeof(linha), memoria) != NULL) { //enquanto nao acabar o arquivo
            sscanf(linha, "%hX", &endereco_temp);
            ultimo_endereco = endereco_temp;
        }
        fclose(memoria);

        memoria = fopen("mem.out","a");
        unsigned short proximo_endereco = ultimo_endereco + 2;
        fprintf(memoria, "%04X %04X\n", proximo_endereco, endereco_memoria1);
        fclose(memoria);

        printf("%04X\n", proximo_endereco);
    }

    return 0;
}
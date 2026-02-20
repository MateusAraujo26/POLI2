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
Crie um programa loop (loop.c) que recebe uma cadeia de valores hexadecimais, 
digitados na mesma linha e separados por espaços (o último valor sem espaço no final). 
Enquanto a diferença entre o valor do acumulador e o valor de entrada 
(valor do acumulador - valor de entrada) for positiva (ou zero), 
o programa armazena esse valor de entrada em mem.out na próxima posição de memória livre. 
Caso o valor seja negativo, o programa termina e imprime o último endereço de memória ocupado. 
Um exemplo de chamada de loop é mostrado a seguir

$ ./loop 56CD 98AF 0741 DF12 25FF
*/

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    unsigned short acumulador;
    FILE *acc = fopen("acc.out", "r");
    fscanf(acc, "%X", &acumulador);
    fclose(acc);

    unsigned short ultimo_endereco = 0;
    unsigned short endereco_temp;
    char linha[20];
    FILE* memoria = fopen("mem.out","r");
    while (fgets(linha, sizeof(linha), memoria) != NULL) { //enquanto nao acabar o arquivo
        sscanf(linha, "%hX", &endereco_temp);
        ultimo_endereco = endereco_temp;
    }
    fclose(memoria);

    for (int i = 1; i < argc; i++){
        unsigned short entrada;
        sscanf(argv[i], "%X", &entrada);

        unsigned short diferenca = (acumulador + (~entrada + 1));

        if ((diferenca & 0x8000) != 0){ // bit mais significativo eh 0 quando positivo
            printf("%04X\n", ultimo_endereco);
            return 0;
        }

        ultimo_endereco += 2;
        memoria = fopen("mem.out", "a");
        fprintf(memoria, "%04X %04X\n", ultimo_endereco, entrada);
        fclose(memoria);
    }
    return 0;
}
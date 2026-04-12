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
Crie um programa sub (sub.c), que lê dois endereços de memória, passados como parâmetro, na mesma linha da chamada do programa, 
separados por um espaço, e calcula o resultado da subtração em hexadecimal com quatro dígitos usando a representação de complemento de 2. 
Em seguida, sub armazena o resultado em um registrador acumulador e também imprime o valor em tela. 
Esse acumulador deve ser um arquivo chamado acc.out. Um exemplo de chamada de sub é mostrado a seguir:

$ ./sub 0000 0004
*/

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    unsigned short param1, param2;
    sscanf(argv[1], "%hX", &param1);
    sscanf(argv[2], "%hX", &param2);

    unsigned short valor1 = param1, valor2 = param2;
    
    FILE *memoria = fopen("mem.out", "r");
    if (memoria != NULL) {
        unsigned short endereco_lido, valor_lido;
        char linha[20];
        int found1 = 0, found2 = 0;

        while (fgets(linha, sizeof(linha), memoria) != NULL) {
            sscanf(linha, "%hX %hX", &endereco_lido, &valor_lido);
            
            if (endereco_lido == param1) {
                valor1 = valor_lido;
                found1 = 1;
            }
            if (endereco_lido == param2) {
                valor2 = valor_lido;
                found2 = 1;
            }
        }
        fclose(memoria);
        
        if (!found1) valor1 = param1;
        if (!found2) valor2 = param2;
    }

    unsigned short resultado = valor1 + (~valor2 + 1);

    FILE *acc = fopen("acc.out", "w");
    fprintf(acc, "%04X\n", resultado);
    fclose(acc);
    
    printf("%04X\n", resultado);

    return 0;
}
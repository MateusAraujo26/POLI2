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
Crie um programa seek que recebe um valor hexadecimal e procura esse valor na memória. 
Se encontrar o valor, imprime o endereço correspondente e coloca o valor no acumulador. 
Caso não encontre, seek retorna o valor 8000 e coloca esse valor no acumulador.

Execute o programa seek passando o valor 1CCC. 
Em seguida, copie o arquivo acc.out nomeado como acc-4-1.out.

Veja um exemplo de chamada de seek a seguir:
$ ./seek ABCD
*/

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    unsigned short valor_busca;
    sscanf(argv[1], "%X", &valor_busca);    
    
    unsigned short endereco, valor;
    char linha[20];
    int encontrado = 0;
    unsigned short endereco_encontrado;
    FILE* memoria = fopen("mem.out", "r");
    while (fgets(linha, sizeof(linha), memoria) != NULL) {
        sscanf(linha, "%hX %hX", &endereco, &valor);
        
        if (valor == valor_busca) {
            encontrado = 1;
            endereco_encontrado = endereco;
            break;
        }
    }
    fclose(memoria);

    FILE *acc = fopen("acc.out", "w");
    if (encontrado) {
        printf("%04X\n", endereco_encontrado);
        fprintf(acc, "%04X\n", valor_busca);
    } else {
        printf("8000\n");
        fprintf(acc, "8000\n");
    }
    fclose(acc);
    
    return 0;
}
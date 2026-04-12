/*    
 \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__

  AO PREENCHER ESSE CABEÇALHO COM O MEU NOME E O MEU NÚMERO USP, 
  DECLARO QUE SOU O ÚNICO AUTOR E RESPONSÁVEL POR ESSE PROGRAMA. 
  TODAS AS PARTES ORIGINAIS DESSE EXERCÍCIO-PROGRAMA (EP) FORAM 
  DESENVOLVIDAS E IMPLEMENTADAS POR MIM SEGUINDO AS INSTRUÇÕES DESSE EP
  E QUE PORTANTO NÃO CONSTITUEM PLÁGIO. DECLARO TAMBÉM QUE SOU RESPONSÁVEL
  POR TODAS AS CÓPIAS DESSE PROGRAMA E QUE EU NÃO DISTRIBUI OU FACILITEI A
  SUA DISTRIBUIÇÃO. ESTOU CIENTE QUE OS CASOS DE PLÁGIO SÃO PUNIDOS COM 
  REPROVAÇÃO DIRETA NA DISCIPLINA.

  Nome: Gabriel Agra de Castro Motta
  NUSP: 15452743

  main.c

  Referências:
 \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/
  

#include <stdio.h>
#include <string.h>
#include <math.h>
/*to colocando math.h so pra ter funcao de raiz quadrada*/

#define TAMANHO 200

FILE *fractal;

void anti_koch(int ordem, float tamanho){
    if(ordem==0)
        fprintf(fractal, "%f 0 rlineto\n", tamanho);
    else{
            tamanho /= 3;
            anti_koch(ordem - 1, tamanho);
            fprintf(fractal, "60 rotate\n");
            anti_koch(ordem - 1, tamanho);
            fprintf(fractal, "-120 rotate\n");
            anti_koch(ordem - 1, tamanho);
            fprintf(fractal, "60 rotate\n");
            anti_koch(ordem - 1, tamanho);
    }
}


void anti_floco_neve(int ordem, float tamanho) {
    int vezes;
    for(vezes = 3; vezes != 0; vezes--){
        anti_koch(ordem, tamanho);
        fprintf(fractal, "120 rotate\n");
    }
}

void canopy(int ordem, float tamanho) {
    if (ordem == 0) {
        fprintf(fractal, "0 %f rlineto\n", tamanho);
    } else {
        fprintf(fractal, "0 %f rlineto\n", tamanho);
        fprintf(fractal, "20 rotate\n");
        canopy(ordem - 1, tamanho * 0.75);
        fprintf(fractal, "0 -%f rlineto\n", tamanho * 0.75);
        fprintf(fractal, "-40 rotate\n");
        canopy(ordem - 1, tamanho * 0.75);
        fprintf(fractal, "0 -%f rlineto\n", tamanho * 0.75);
        fprintf(fractal, "20 rotate\n");
    }
}

void curva_dragao_auxiliar(int ordem, float tamanho, int direcao) {
    if (ordem == 0) {
        fprintf(fractal, "%f 0 rlineto\n", tamanho);
    } else {
        curva_dragao_auxiliar(ordem - 1, tamanho * sqrt(2), 1);
        fprintf(fractal, "%d rotate\n", 45 * direcao);
        curva_dragao_auxiliar(ordem - 1, tamanho * sqrt(2), -1);
    }
}

/*uso essa funcao auxiliae pra poder usar um parametro a mais: a direcao binarias*/
void curva_dragao(int ordem, float tamanho) {
    curva_dragao_auxiliar(ordem, tamanho, 1);
}

void novo_fractal(int ordem, float tamanho) {
    if (ordem == 0) {
        fprintf(fractal, "0 %f rlineto\n", tamanho);
    } else {
        fprintf(fractal, "%f %f rlineto\n", tamanho);
        fprintf(fractal, "20 rotate\n");
        canopy(ordem - 1, tamanho * 0.75);
        fprintf(fractal, "%f -%f rlineto\n", tamanho * 0.75);
        fprintf(fractal, "-60 rotate\n");
        canopy(ordem - 1, tamanho * 0.75);
        fprintf(fractal, "%f -%f rlineto\n", tamanho * 0.75);
        fprintf(fractal, "40 rotate\n");
    }
}

int main() {
    int tipo, ordem;
    char nome[50] = {"\0"};

    printf("Entre com o tipo de fractal: ");
    scanf("%d", &tipo);
    printf("Entre com a ordem: ");
    scanf("%d", &ordem);
    printf("Entre com o nome do arquivo: ");
    scanf(" %s", nome);
    strcat(nome, ".ps");
    fractal = fopen(nome, "w");

    switch(tipo) {
        case 0:
            fprintf(fractal, "200 300 moveto\n");
            anti_floco_neve(ordem, TAMANHO);
            break;
        case 1:
            fprintf(fractal, "300 100 moveto\n");
            canopy(ordem, TAMANHO);
            break;
        case 2:
            fprintf(fractal, "0 200 moveto\n");
            curva_dragao(ordem, TAMANHO);
            break;
        case 3:
            fprintf(fractal, "0 200 moveto\n");
            novo_fractal(ordem, TAMANHO);
            break;
    }
    fprintf(fractal, "stroke");
    fclose(fractal);
}
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

  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma referência, liste-as abaixo
  para que o seu programa não seja considerada plágio.
  Exemplo:

  - biblioteca stack e funcoes *infixaParaPosfixa e fazOperacao adaptadas de:
       http://www.ime.usp.br/~cris/mac0122/slides/aula11.pdf

  - funcao mallocsafe adaptada de:
       https://www.ime.usp.br/~coelho/mac0122-2014/aulas/aula06-3x2.pdf

 \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stack.h"
#include "polinomios.h"

int main()
{
    printf("*************************\nCalculadora de polinomios\n*************************\n");

    char str[2] = {'\0'};
    /*criamos todos os polinomios apos inicializacao da main*/
    polinomio var[26];
    for (int i = 0; i < 26; i++)
    {
        var[i] = cria();
    }

    while (1)
    {
        printf("Digite uma expressão ou quit para sair do programa:\n> ");
        scanf(" %2s", str);
        char seletor_de_polinomio = str[0];
        char operacao = str[1];

        /*usamos var[index] para acessar a posicao correspondente do polinomio*/
        int index = seletor_de_polinomio - 'a';

        /*se o usuario digitar quit, fechamos o programa com return 0*/
        /*lemos os primeiros dois caracteres, mas so lemos o resto da frase caso
        esses caracteres sejam "qu", e entao verificamos se foi digitado "quit"*/
        if (strcmp(str, "qu") == 0)
        {
            char palavra[3];
            scanf("%2s", palavra);
            if (strcmp(palavra, "it") == 0)
            {
                printf("Tchau!\n");
                return 0;
            }
        }

        switch (operacao)
        {
        case '?':
            impr(seletor_de_polinomio, var[index]);
            break;
        case ':':
            var[index] = leia();
            impr(seletor_de_polinomio, var[index]);
            break;
        case '=':
        {
            char expr[100];
            char *posfixa;
            scanf(" %99s", expr);
            /*guardamos expressao posfixa devolvida*/
            posfixa = infixaParaPosfixa(expr);
            /*usamos a expressao posfixa para calcular e atribuir o resultado ao polinomio*/
            var[index] = calculaExpressao(posfixa, var);
            /*libera posfixa*/
            free(posfixa);

            impr(seletor_de_polinomio, var[index]);
            break;        
        }
        break;
        }
    }
}
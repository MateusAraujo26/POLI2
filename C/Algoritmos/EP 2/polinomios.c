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

  - função mallocSafe copiada de: 

       http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html

 \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/

#include <stdio.h>
#include <stdlib.h>
#include "polinomios.h"

/* declaracao da celula das listas encadeadas */
typedef struct celula{
    int coeficiente;
    int expoente;
    struct celula* proximo;
} celula;

/* prototipo de eventuais funcoes auxiliares */
celula* criar_nova_celula(float coeficiente, int expoente);

void novo_termo(celula** cabeca, float coeficiente, int expoente);

/* implementacao das funcoes da biblioteca polinomios.h */
polinomio cria() {
    celula* cabeca = criar_nova_celula(0, 0);
    return (polinomio)cabeca;
}

polinomio leia() {
    celula* cabeca = NULL;
    float coeficiente;
    int expoente;

    while (1) {
        /*quando o coeficiente for 0, saimos do loop*/
        scanf("%f", &coeficiente);
        if (coeficiente == 0) {
            break;
        }
        scanf("%d", &expoente);
        novo_termo(&cabeca, coeficiente, expoente);
    }
    return cabeca;
}

polinomio copia(polinomio p) {
    celula* cabeca_da_nova = NULL;
    celula* polinomio_original = (celula*)p;
    while (polinomio_original != NULL) {
        novo_termo(&cabeca_da_nova, polinomio_original->coeficiente, polinomio_original->expoente);
        polinomio_original = polinomio_original->proximo;
    }
    return cabeca_da_nova;
}

void impr(char c, polinomio p) {
    celula* polinomio = (celula*)p;
    printf("%c(x) = ", c);
    while (polinomio != NULL) {
        if (polinomio->coeficiente == 0) {
            printf("0");
        } else {
            if (polinomio->expoente == 0) {
                printf("%.2f", (float)polinomio->coeficiente);
            } else {
                printf("%.2f x^%d", (float)polinomio->coeficiente, polinomio->expoente);
            }
        }
        polinomio = polinomio->proximo;
        if (polinomio != NULL) { /*printa os termos e o sinal de soma ate acabar o polinomio*/
            printf(" + ");
        }
    }
    printf("\n");
}

polinomio soma(polinomio p, polinomio q) {
    celula* cabeca_da_soma = NULL;
    celula* polinomio_p = (celula*)p;
    celula* polinomio_q = (celula*)q;

    while (polinomio_p != NULL && polinomio_q != NULL) {
        if (polinomio_p->expoente == polinomio_q->expoente) { 
            /*tratamos os tres casos: se e somente se, serem iguais, somamos os coeficientes antes de criar o novo*/
            novo_termo(&cabeca_da_soma, polinomio_p->coeficiente + polinomio_q->coeficiente, polinomio_p->expoente);
            polinomio_p = polinomio_p->proximo;
            polinomio_q = polinomio_q->proximo;
        } else if (polinomio_p->expoente > polinomio_q->expoente) { /*p maior, criamos termo de p*/
            novo_termo(&cabeca_da_soma, polinomio_p->coeficiente, polinomio_p->expoente);
            polinomio_p = polinomio_p->proximo;
        } else { /*q maior, criamos termo de q*/
            novo_termo(&cabeca_da_soma, polinomio_q->coeficiente, polinomio_q->expoente);
            polinomio_q = polinomio_q->proximo;
        }
    }
    /*se um dos polinomios acabar, percorremos o outro ate ele acabar tbm*/
    while (polinomio_p != NULL) { 
        novo_termo(&cabeca_da_soma, polinomio_p->coeficiente, polinomio_p->expoente);
        polinomio_p = polinomio_p->proximo;
    }

    while (polinomio_q != NULL) {
        novo_termo(&cabeca_da_soma, polinomio_q->coeficiente, polinomio_q->expoente);
        polinomio_q = polinomio_q->proximo;
    }

    return cabeca_da_soma;
}

polinomio subt(polinomio p, polinomio q) {
    celula* cabeca_da_subtracao = NULL;
    celula* polinomio_p = (celula*)p;
    celula* polinomio_q = (celula*)q;

    /*aqui seguimos a mesma logica da soma*/
    while (polinomio_p != NULL && polinomio_q != NULL) {
        if (polinomio_p->expoente == polinomio_q->expoente) {
            novo_termo(&cabeca_da_subtracao, polinomio_p->coeficiente - polinomio_q->coeficiente, polinomio_p->expoente);
            polinomio_p = polinomio_p->proximo;
            polinomio_q = polinomio_q->proximo;
        } else if (polinomio_p->expoente > polinomio_q->expoente) {
            novo_termo(&cabeca_da_subtracao, polinomio_p->coeficiente, polinomio_p->expoente);
            polinomio_p = polinomio_p->proximo;
        } else {
            novo_termo(&cabeca_da_subtracao, -polinomio_q->coeficiente, polinomio_q->expoente);
            polinomio_q = polinomio_q->proximo;
        }
    }

    while (polinomio_p != NULL) {
        novo_termo(&cabeca_da_subtracao, polinomio_p->coeficiente, polinomio_p->expoente);
        polinomio_p = polinomio_p->proximo;
    }
    /*subtraimos o coeficiente de q do resultado, por estarmos subtraindo o q de p*/
    while (polinomio_q != NULL) {
        novo_termo(&cabeca_da_subtracao, -polinomio_q->coeficiente, polinomio_q->expoente);
        polinomio_q = polinomio_q->proximo;
    }

    return cabeca_da_subtracao;
}

polinomio nega(polinomio p) {
    celula* cabeca_do_negativo = NULL;
    celula* celula_positiva = (celula*)p;
    while (celula_positiva != NULL) {
        novo_termo(&cabeca_do_negativo, -celula_positiva->coeficiente, celula_positiva->expoente);
        celula_positiva = celula_positiva->proximo;
    }
    return cabeca_do_negativo;
}

polinomio mult(polinomio p, polinomio q) {
    celula* cabeca_da_multiplicacao = NULL;
    celula* polinomio_p = (celula*)p;
    celula* polinomio_q = (celula*)q;

    while (polinomio_p != NULL) {
        celula* temporario_q = polinomio_q;
        while (temporario_q != NULL) {
            novo_termo(&cabeca_da_multiplicacao, polinomio_p->coeficiente * temporario_q->coeficiente, polinomio_p->expoente + temporario_q->expoente);
            temporario_q = temporario_q->proximo;
        }
        polinomio_p = polinomio_p->proximo;
    }

    return cabeca_da_multiplicacao;
}

polinomio quoc(polinomio p, polinomio q) {
    celula* cabeca_do_quociente = NULL;
    celula* dividendo = (celula*)p;
    celula* divisor = (celula*)q;

    while (dividendo != NULL && dividendo->expoente >= divisor->expoente && dividendo->coeficiente != 0) {
        if (dividendo->coeficiente == 0) {
            dividendo = dividendo->proximo;
            continue;
        }
        int coeficiente = dividendo->coeficiente / divisor->coeficiente;
        int expoente = dividendo->expoente - divisor->expoente;

        celula* termo_quociente = criar_nova_celula(coeficiente, expoente);
        novo_termo(&cabeca_do_quociente, coeficiente, expoente);

        celula* produto_do_divisor_pelo_resultado = (celula*)mult((polinomio)divisor, (polinomio)termo_quociente);
        celula* resto_da_divisao = (celula*)subt((polinomio)dividendo, (polinomio)produto_do_divisor_pelo_resultado);

        libera((polinomio)produto_do_divisor_pelo_resultado);

        dividendo = resto_da_divisao;
    }

    return (polinomio)cabeca_do_quociente;
}

polinomio rest(polinomio p, polinomio q) { 
    /*chamamos a funcao quociente, pegamos o quociente, multiplicamos pelo divisor. 
    pegamos o dividendo e dele subtraimos a multiplicacao quociente-divisor, esse sera o resto*/

    polinomio quociente = quoc(p, q);
    polinomio produto = mult(quociente, q);

    polinomio cabeca_do_resto = subt(p, produto);

    libera(produto);
    libera(quociente);

    return cabeca_do_resto; 
}

void libera(polinomio p) {
    celula* celula_atual = (celula*)p;
    while (celula_atual != NULL) {
        celula* temporario_percorrido = celula_atual;
        celula_atual = celula_atual->proximo;
        free(temporario_percorrido);
    }
}

/* Implementacao das funcoes auxiliares */

celula* criar_nova_celula(float coeficiente, int expoente) {
    celula* nova_celula = (celula*)malloc(sizeof(celula));
    nova_celula->coeficiente = coeficiente;
    nova_celula->expoente = expoente;
    nova_celula->proximo = NULL;
    return nova_celula;
}

void novo_termo(celula** cabeca, float coeficiente, int expoente) {
    celula* novo = (celula*)malloc(sizeof(celula));
    novo->coeficiente = coeficiente;
    novo->expoente = expoente;
    novo->proximo = NULL;

    if (*cabeca == NULL) {
        *cabeca = novo;
    } else {
        celula* atual = *cabeca;
        celula* anterior = NULL;
        while (atual != NULL && atual->expoente < expoente) {
            anterior = atual;
            atual = atual->proximo;
        }
        
        if (atual != NULL && atual->expoente == expoente) {
            atual->coeficiente += coeficiente;
            free(novo);
        } else {
            if (anterior == NULL) {
                novo->proximo = *cabeca;
                *cabeca = novo;
            } else {
                novo->proximo = atual;
                anterior->proximo = novo;
            }
        }
    }
}
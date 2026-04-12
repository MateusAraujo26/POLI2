#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "polinomios.h"

/*
Definição da estrutura interna para cada celula/monomio
da lista encadeada. 
*/
typedef struct No{
    float coeficiente;   // Coeficiente do termo
    int expoente;        // Expoente do termo
    struct No *prox;     // Apontador para o próximo nó
} No;


/* 
Função que cria e devolve um polinômio nulo, que pode ser representado
por um ponteiro nulo.
*/
polinomio cria(){
    return NULL;
}

/*
Função que percorre cada célula do polinômio e libera ela, até
encontrar uma célula null.
*/
void libera(polinomio p){
    No *atual = (No *)p;
    No *prox;

    while (atual != NULL){
        prox = atual->prox;  
        free(atual);         
        atual = prox;   
    }
}

/*
Função auxiliar privada (que não poderá ser acesseada pelos clientes da biblioteca)
utilizada para inserir um termo ordenadamente conforme o exponente.

Não insere termos com coeficiente zero. (monômios nulos mencionados na correção)

Soma os coeficientes no caso de inserir um termo cujo expoente já está presente em
uma célula de p, evitando problemas como p = x^2 + 2x^2 + x^3. Nesse caso, libera o 
nó criado que é desnecessário.
*/
polinomio inserir_ordenado(polinomio p, float coeficiente, int expoente) {
    if (coeficiente == 0){
        return p; 
    }

    No *novo = (No *)malloc(sizeof(No));

    novo->coeficiente = coeficiente;
    novo->expoente = expoente;
    novo->prox = NULL;

    // Inserção no primeiro elemento:
    if (p == NULL || ((No *)p)->expoente > expoente){
        novo->prox = (No *)p;
        p = novo;
    }
    
    // Demais casos:
    else{
        No *atual = (No *)p;
        No *anterior = NULL;

        while (atual != NULL && atual->expoente < expoente){
            anterior = atual;
            atual = atual->prox;
        }

        // Caso mesmo expoente: 
        if (atual != NULL && atual->expoente == expoente){
            atual->coeficiente += coeficiente;
            free(novo); 

            // Remove no caso de um coeficiente nulo:
            if (atual->coeficiente == 0){
                if (anterior == NULL){
                    p = atual->prox;
                }
                else{
                    anterior->prox = atual->prox;
                }
                free(atual);
            }
        }
        
        else{
            anterior->prox = novo;
            novo->prox = atual;
        }
    }

    return p;
}

/*
Função privada para inserir sabendo qual é o último nó lido, evitando
percorrer desnecessariamente a lista desde o começo
*/
void insere(polinomio *resultado, No **ultimo, float coeficiente, int expoente) {
    if (coeficiente == 0) {
        return; 
    }

    No *novo = (No *)malloc(sizeof(No));

    novo->coeficiente = coeficiente;
    novo->expoente = expoente;
    novo->prox = NULL;

    if (*resultado == NULL){
        *resultado = novo;
        *ultimo = novo;
    }
    else{
        (*ultimo)->prox = novo;
        *ultimo = novo;
    }
}

/*
Função leia(): cria um polinômio e preenche ele
a partir da leitura de coeficientes e expoentes
da entrada padrão, parando ao encontrar um coeficiente
nulo.

Os polinômios são preenchidos com células ordenadas
por expoente.
*/
polinomio leia() {
    float coeficiente;
    int expoente;
    polinomio p = cria(); 
    while (1){
        scanf("%f", &coeficiente);
        if (coeficiente == 0) {
            break; 
        }
        scanf("%d", &expoente);
        p = inserir_ordenado(p, coeficiente, expoente);
    }
    return p;
}

/*
Função copia(): cria um novo polinômio e percorre o 
polinomio p passado como parametro copiando celula
por celula.
*/
polinomio copia(polinomio p) {
    if (p == NULL) {
        return NULL; 
    }

    polinomio copia = cria(); 
    No *atual = (No *)p;   
    No *novo, *ultimo = NULL;

    while (atual != NULL){
        novo = (No *)malloc(sizeof(No));
        
        novo->coeficiente = atual->coeficiente;
        novo->expoente = atual->expoente;
        novo->prox = NULL;

        if (copia == NULL) {
            copia = novo;
        }
        else{
            ultimo->prox = novo;
        }
        ultimo = novo;
        atual = atual->prox;
    }
    return copia;
}

/*
Função impr(): imprime o nome do polinomio seguido pelo
seu valor. Coeficientes com 2 casas decimais.

Observe que não há verificação para evitar monomios de 
coeficiente zero, pois já foram tratados durante a criação
do polinômio.

Além disso, há um caso específico para o polinômio nulo.
Pois ele sequer entra no loop de impressão. 

Questões extras que não foram desconsideradas na correção:

Aqui, há um tratamento para imprimir apenas um sinal 
(evitar + - 2x^2, por exemplo). E também para evitar
imprimir p = +x^3.

A função também verifica os expoentes, imprimindo x se
o expoente for 1 e não imprimindo x se o expoente for 
zero. Expoentes negativos não foram solicitados no enunciado.
*/
void impr(char c, polinomio p){
    printf("%c(x) = ", c);
    No *atual = (No *)p;
    if (atual == NULL){
        printf("0\n");
        return;
    }

    int primeiroTermo = 1; 
    
    while (atual != NULL){
        if (atual->coeficiente != 0){
            if (!primeiroTermo) {
                if (atual->coeficiente > 0){
                    printf(" + ");
                } else {
                    printf(" - ");
                }
            }
            else{
                if (atual->coeficiente < 0){
                    printf("-");
                }
            }
            printf("%.2f", fabs(atual->coeficiente));

            if (atual->expoente == 1){
                printf("x");
            }
            else if (atual->expoente > 1){
                printf("x^%d", atual->expoente);
            }

            primeiroTermo = 0;
        }
        atual = atual->prox;
    }

    printf("\n");
}

/*
Função soma(): cria um polinômio e percorre
p e q utilizando a ordem crescente para somar
expoentes iguais.
*/
polinomio soma(polinomio p, polinomio q) {
    No *atual_p = (No *)p;
    No *atual_q = (No *)q;
    polinomio resultado = cria();
    No *ultimo = NULL; 

    while (atual_p != NULL && atual_q != NULL){
        if (atual_p->expoente < atual_q->expoente){
            insere(&resultado, &ultimo, atual_p->coeficiente, atual_p->expoente);
            atual_p = atual_p->prox;
        }
        else if (atual_p->expoente > atual_q->expoente){
            insere(&resultado, &ultimo, atual_q->coeficiente, atual_q->expoente);
            atual_q = atual_q->prox;
        }
        else{
            float soma_coef = atual_p->coeficiente + atual_q->coeficiente;
            insere(&resultado, &ultimo, soma_coef, atual_p->expoente);
            atual_p = atual_p->prox;
            atual_q = atual_q->prox;
        }
    }

    while (atual_p != NULL){
        insere(&resultado, &ultimo, atual_p->coeficiente, atual_p->expoente);
        atual_p = atual_p->prox;
    }

    while (atual_q != NULL){
        insere(&resultado, &ultimo, atual_q->coeficiente, atual_q->expoente);
        atual_q = atual_q->prox;
    }

    return resultado;
}

/*
Função nega(): troca o sinal dos coeficientes. Igual
a função copia mas com a troca de sinal.
*/
polinomio nega(polinomio p){
    if (p == NULL) {
        return NULL; 
    }

    polinomio neg = cria(); 
    No *atual = (No *)p;   
    No *novo, *ultimo = NULL;

    while (atual != NULL){
        novo = (No *)malloc(sizeof(No));
        
        novo->coeficiente = -(atual->coeficiente);
        novo->expoente = atual->expoente;
        novo->prox = NULL;

        if(neg == NULL){
            neg = novo;
        }
        else{
            ultimo->prox = novo;
        }
        ultimo = novo;
        atual = atual->prox;
    }
    return neg;
}

/*
Função subt(): realiza a subtração da mesma forma
que a função soma, mas inverte o sinal dos elemntos 
de q. Evitamos utilizar a função nega para não percorrer
q uma vez a mais.
*/
polinomio subt(polinomio p, polinomio q) {
    No *atual_p = (No *)p;
    No *atual_q = (No *)q;
    polinomio resultado = cria();
    No *ultimo = NULL; 

    while (atual_p != NULL && atual_q != NULL){
        if (atual_p->expoente < atual_q->expoente){
            insere(&resultado, &ultimo, atual_p->coeficiente, atual_p->expoente);
            atual_p = atual_p->prox;
        }
        else if (atual_p->expoente > atual_q->expoente){
            insere(&resultado, &ultimo, -(atual_q->coeficiente), atual_q->expoente);
            atual_q = atual_q->prox;
        }
        else{
            float soma_coef = atual_p->coeficiente - atual_q->coeficiente;
            insere(&resultado, &ultimo, soma_coef, atual_p->expoente);
            atual_p = atual_p->prox;
            atual_q = atual_q->prox;
        }
    }

    while (atual_p != NULL){
        insere(&resultado, &ultimo, atual_p->coeficiente, atual_p->expoente);
        atual_p = atual_p->prox;
    }

    while (atual_q != NULL){
        insere(&resultado, &ultimo, -(atual_q->coeficiente), atual_q->expoente);
        atual_q = atual_q->prox;
    }

    return resultado;
}

/*
Função mult(): cria um polinômio e percorre p 
para cada elemento de q, multiplicando os coeficientes
e somando os expoentes. Utiliza inserir_ordenado
para mesclar termos de mesmo coeficiente e manter
o resultado ordenado por expoente.
*/
polinomio mult(polinomio p, polinomio q){
    if(p == NULL || q == NULL){
        return cria();
    }
    polinomio resultado = cria();
    No *atual_p = (No *)p;

    while (atual_p != NULL){
        No *atual_q = (No *)q;

        while (atual_q != NULL) {
            float coef_produto = atual_p->coeficiente * atual_q->coeficiente;
            int exp_soma = atual_p->expoente + atual_q->expoente;
            resultado = inserir_ordenado(resultado, coef_produto, exp_soma);
            atual_q = atual_q->prox;
        }

        atual_p = atual_p->prox;
    }

    return resultado;
}

/*
Função quoc(): divide dois polinomios, começando dos maiores coeficientes
Observe que esta função seria mais eficiente se os polinômios
estivessem ordenados de forma decrescente, ao contrário do que
foi implementado aqui.

Começa com uma cópia de p no resto e subtrai os produtos
até que não seja mais possível dividir. 

A função rest(), abaixo, faz a mesma coisam, mas retorna o resto.
Uma estratégia comum para calcular o resto, para alunos que 
implementaram quoc com sucesso, foi de inverter a conta:

a = b * q + r -> r = a - b * q

ou seja rest(p, q) = subt(p, mult(q, quoc(p, q))

Observe que nesse caso cada polinomio auxiliar da operação deveria
ser devidamente declarado e liberado após o uso.
*/
polinomio quoc(polinomio p, polinomio q){
    if (q == NULL){
        printf("Erro: divisão por polinômio nulo\n");
        return cria();  
    }

    polinomio quociente = cria();  
    polinomio resto = copia(p);   

    No *termo_divisor = NULL;

    termo_divisor = (No *)q;
    while (termo_divisor->prox != NULL){
        termo_divisor = termo_divisor->prox;
    }

    while (resto != NULL){
        No *termo_resto = (No *)resto;
        termo_resto = (No *)resto;
        while (termo_resto->prox != NULL) {
            termo_resto = termo_resto->prox;
        }

        if (termo_resto->expoente < termo_divisor->expoente){
            break; 
        }

        float coef_termo = termo_resto->coeficiente / termo_divisor->coeficiente;
        int exp_termo = termo_resto->expoente - termo_divisor->expoente;

        quociente = inserir_ordenado(quociente, coef_termo, exp_termo);

        polinomio produto = cria();
        No *atual_q_temp = (No *)q;
        while (atual_q_temp != NULL) {
            float coef_produto = coef_termo * atual_q_temp->coeficiente;
            int exp_produto = exp_termo + atual_q_temp->expoente;

            produto = inserir_ordenado(produto, coef_produto, exp_produto);

            atual_q_temp = atual_q_temp->prox;
        }
        polinomio novo_resto = subt(resto, produto);

        libera(produto);
        libera(resto);

        resto = novo_resto;
    }
    libera(resto);  
    return quociente;  
}

polinomio rest(polinomio p, polinomio q) {
    if (q == NULL) {
        printf("Erro: divisão por polinômio nulo\n");
        return cria();  
    }

    polinomio resto = copia(p);  

    No *termo_divisor = NULL;

    termo_divisor = (No *)q;
    while (termo_divisor->prox != NULL) {
        termo_divisor = termo_divisor->prox;
    }

    while (resto != NULL) {
        No *termo_resto = (No *)resto;
        termo_resto = (No *)resto;
        while (termo_resto->prox != NULL) {
            termo_resto = termo_resto->prox;
        }

        if (termo_resto->expoente < termo_divisor->expoente) {
            break; 
        }

        float coef_termo = termo_resto->coeficiente / termo_divisor->coeficiente;
        int exp_termo = termo_resto->expoente - termo_divisor->expoente;

        polinomio produto = cria();
        No *atual_q_temp = (No *)q;
        while (atual_q_temp != NULL) {
            float coef_produto = coef_termo * atual_q_temp->coeficiente;
            int exp_produto = exp_termo + atual_q_temp->expoente;

            produto = inserir_ordenado(produto, coef_produto, exp_produto);

            atual_q_temp = atual_q_temp->prox;
        }
        polinomio novo_resto = subt(resto, produto);

        libera(produto);
        libera(resto);

        resto = novo_resto;
    }

    return resto; 
}
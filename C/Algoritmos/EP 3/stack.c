#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "stack.h"
#include "polinomios.h"

typedef struct stackNode *Link;
typedef struct stack *TStack;

struct stackNode
{
    Item conteudo;
    Link prox;
};

struct stack
{
    Link topo;
};

void *mallocSafe(size_t nbytes)
{
    void *ponteiro = malloc(nbytes);
    if (ponteiro == NULL)
    {
        printf("Socorro! malloc devolveu "
               "NULL!\n");
        exit(EXIT_FAILURE);
    }
    return ponteiro;
}

Stack stackInit()
{
    TStack s = (TStack)mallocSafe(sizeof *s);
    s->topo = NULL;
    return (Stack)s;
}

int stackEmpty(Stack s)
{
    return ((TStack)s)->topo == NULL;
}

void stackPush(Stack s, Item item)
{
    Link nova = (Link)mallocSafe(sizeof *nova);
    nova->conteudo = item;
    nova->prox = ((TStack)s)->topo;
    ((TStack)s)->topo = nova;
}

Item stackPop(Stack s)
{
    Link p = ((TStack)s)->topo;
    Item conteudo = p->conteudo;
    ((TStack)s)->topo = p->prox;
    free(p);
    return conteudo;
}

Item stackTop(Stack s)
{
    return ((TStack)s)->topo->conteudo;
}

void stackFree(Stack s)
{
    while (((TStack)s)->topo != NULL)
    {
        Link p = ((TStack)s)->topo;
        ((TStack)s)->topo = p->prox;
        free(p);
    }
    free(s);
}

char *infixaParaPosfixa(char *infixa)
{
    char *posfixa; /* expressao polonesa */
    int n = strlen(infixa);
    int i;
    int j;
    char topo; /* item do topo da stack */
    /*aloca area para expressao polonesa*/
    posfixa = (char *)mallocSafe((n + 1) * sizeof(char));
    /* O '+1' eh para o '\0' */
    Stack stack = stackInit(); /* inicializa a stack */
    /* examina cada item da infixa */
    for (i = j = 0; i < n; i++)
    {
        switch (infixa[i])
        {
        case '(':
            stackPush(stack, (Item)(intptr_t)infixa[i]);
            break;
        case ')':
            while ((topo = (char)(intptr_t)stackPop(stack)) != '(')
                posfixa[j++] = topo;
            break;
        case '+':
        case '-':
            while (!stackEmpty(stack) && (topo = (char)(intptr_t)stackTop(stack)) != '(')
                posfixa[j++] = (char)(intptr_t)stackPop(stack);
            stackPush(stack, (Item)(intptr_t)infixa[i]);
            break;
        case '*':
        case '/':
            while (!stackEmpty(stack) && (topo = (char)(intptr_t)stackTop(stack)) != '(' && topo != '+' && topo != '-')
                posfixa[j++] = (char)(intptr_t)stackPop(stack);
            stackPush(stack, (Item)(intptr_t)infixa[i]);
            break;
        default:
            if (infixa[i] != ' ')
                posfixa[j++] = infixa[i];
        } /* fim switch */
    } /* fim for (i=j=0...) */
    /* desempilha todos os operandos que restaram */
    while (!stackEmpty(stack))
        posfixa[j++] = (char)(intptr_t)stackPop(stack);
    posfixa[j] = '\0'; /* fim expr polonesa */
    stackFree(stack);
    return posfixa;
} /* fim funcao */

void fazOperacao(Stack pilha_valores, char op)
{
    polinomio operando2 = (polinomio)stackPop(pilha_valores);
    polinomio operando1;
    polinomio resultado_operacao;
    int nao_eh_negado = (op != '~');

    nao_eh_negado ? operando1 = (polinomio)stackPop(pilha_valores) : 0;

    switch (op)
    {
    case '+':
        resultado_operacao = soma(operando1, operando2);
        break;
    case '-':
        resultado_operacao = subt(operando1, operando2);
        break;
    case '*':
        resultado_operacao = mult(operando1, operando2);
        break;
    case '/':
        resultado_operacao = quoc(operando1, operando2);
        break;
    case '%':
        resultado_operacao = rest(operando1, operando2);
        break;
    case '~':
        resultado_operacao = nega(operando2);
        break;
    }

    stackPush(pilha_valores, resultado_operacao);
    libera(operando2);
    if (nao_eh_negado)
    {
        libera(operando1);
    }
}

void *calculaExpressao(char *posfixa, void *var[])
{
    int n = strlen(posfixa);
    int i;
    void *resultado;
    Stack pilha_valores = stackInit();

    for (i = 0; i < n; i++)
    {
        char c = posfixa[i];
        if (c == ' ')
        {
            continue;
        }

        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '~')
        {
            fazOperacao(pilha_valores, c);
        }
        else{
            int index = c - 'a';
            void *operando = copia(var[index]);
            stackPush(pilha_valores, operando);
        }
    }

    resultado = stackPop(pilha_valores);
    stackFree(pilha_valores);
    return resultado;
}
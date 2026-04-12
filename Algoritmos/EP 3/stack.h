/*
* stack.h
* INTERFACE: funcoes para manipular pilhas
*/
#include "item.h"

typedef void *Stack;

Stack stackInit();
int stackEmpty(Stack);
void stackPush(Stack, Item);
Item stackPop(Stack);
Item stackTop(Stack);
void stackFree(Stack);

char *infixaParaPosfixa(char *);
void *calculaExpressao(char *, void *var[]);
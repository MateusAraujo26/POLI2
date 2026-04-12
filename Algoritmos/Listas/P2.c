#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define TRUE 1
#define FALSE 0

static void *
mallocSafe (unsigned int n) 
{
  void *p;

  p = malloc(n);
  if (p == NULL)
    {
      fprintf(stderr,"stack: malloc de %u bytes falhou.\n", n);
      exit (-1);
    }
  return p;
}


/* RECURSAO */

/*Escreva uma função recursiva com protótipo
   int num_digitos (int n);
que devolve o número de dígitos de n.*/
int num_digitos(int n) {
    if (n < 10) return 1;
    return 1 + num_digitos(n / 10);
}

/*Escreva uma função recursiva com protótipo
   int soma_digitos (int n);
que devolve a soma do número de dígitos de n.*/
int soma_digitos (int n){
    if(n == 0) return 0;
    else return (n % 10 + soma_digitos(n / 10));
}

/*Escreva uma função recursiva com protótipo
   int conta_digitos (int n, int d);
onde 0 <= d <= 9, e devolve o número de vezes que d aparece como dígito em n.*/
int conta_digitos (int n, int d){
    if(n < 10) return (n == d) ? 1 : 0;
    int count = 0;
    if(n % 10 == d) count++;
    return count + conta_digitos(n / 10, d);
}

/*Escreva uma função recursiva com protótipo
   int conta (int v[], int n, int x);
que devolve o número de vezes que o inteiro x ocorre no vetor v[0..n-1].*/
int conta (int v[], int n, int x){
    if (n == 0) return 0;
    int count = 0;
    if(v[n-1] == x) count++;
    return count + conta(v, n - 1, x);
}

/*Escreva uma função recursiva com protótipo
   int soma (int v[], int n);
que devolve a soma dos números no vetor v[0..n-1].*/
int soma (int v[], int n){
    if (n == 0) return 0;
    return v[n - 1] + soma(v, n - 1);
}

/*Escreva uma função recursiva com protótipo
   int busca (int v[], int n, int x);
que devolve -1 se x não ocorre em v[0..n-1] ou devolve um índice i com 0 <= i < n tal que v[i] = x.*/
int busca (int v[], int n, int x){
    if (n == 0) return -1;
    if (v[n - 1] == x) return (n - 1);
    return busca(v, n - 1, x);
}

/*Escreva uma função recursiva com protótipo
   int minimo (int v[], int n);
que devolve o índice de um elemento mínimo do vetor v[0..n-1].*/
int minimo (int v[], int n){
    if (n < 2) return 0;
    int minimo_index = minimo(v, n - 1);
    return (v[n - 1] < v[minimo_index]) ? (n - 1) : minimo_index;
}

/*Escreva uma função recursiva com protótipo
  void maxmin (int v[], int n, int *max, int *min);
que devolve o maior valor do vetor v[0..n-1] em *max e o menor em *min.*/
void maxmin (int v[], int n, int *max, int *min){
    if (n == 1){
        *max = v[0];
        *min = *max;
        return;
    }

    maxmin(v, n - 1, max, min);

    if(v[n - 1] < *min) *min = v[n - 1];
    if(v[n - 1] > *max) *max = v[n - 1];
}

/* LISTAS ENCADEADAS */

typedef struct Celula{
    int valor;     
    struct Celula *proximo;
} Celula;

/*Escreva uma função com protótipo
int soma (Celula *ini);
que recebe uma lista encadeada ini de números inteiros e devolve a soma dos números na lista. Suponha que a lista encadeada não tem cabeça de lista.*/
int soma(Celula *ini) {
    int total = 0;
    Celula *atual = ini;
    while (atual != NULL) {
        total += atual->valor;
        atual = atual->proximo;
    }
    return total;
}

/*Escreva uma função com protótipo
int conta (Celula *ini, int x);
que recebe uma lista encadeada ini de números inteiros e um inteiro x, e devolve o número de vezes que x aparece na lista. Suponha que a lista encadeada não tem cabeça de lista.*/
int conta(Celula *ini, int x) {
    int contador = 0;
    Celula *atual = ini;
    while (atual != NULL) {
        if (atual->valor == x) {
            contador++;
        }
        atual = atual->proximo;
    }
    return contador;
}

/*Escreva uma função com protótipo
int crescente (Celula *ini);
que recebe uma lista encadeada ini de números inteiros e devolve TRUE se a lista está em ordem crescente e FALSE caso contrário. Suponha que a lista encadeada não tem cabeça de lista e que contém pelo menos um elemento.*/
int crescente(Celula *ini) {
    if (ini == NULL || ini->proximo == NULL) {
        return TRUE;
    }
    Celula *atual = ini;
    while (atual->proximo != NULL) {
        if (atual->valor > atual->proximo->valor) {
            return FALSE;
        }
        atual = atual->proximo;
    }
    return TRUE;
}

/*Escreva uma função com protótipo
void acrescenta (Celula *ini, int delta);
que recebe uma lista encadeada ini de números inteiros e soma delta no conteúdo de cada elemento da lista, removendo elementos cujo resultado da soma seja nulo. Suponha que a lista encadeada tem cabeça de lista.*/
void acrescenta(Celula *ini, int delta) {
    Celula *atual = ini->proximo; // Começa após a cabeça da lista
    Celula *anterior = ini;

    while (atual != NULL) {
        atual->valor += delta;
        if (atual->valor == 0) {
            anterior->proximo = atual->proximo;
            free(atual);
            atual = anterior->proximo;
        } else {
            anterior = atual;
            atual = atual->proximo;
        }
    }
}

/*Escreva uma função com protótipo
void remove_todos (Celula *ini, int x);
que remove todas as cópias de x da lista encadeada apontada por ini. Suponha que a lista tem cabeça de lista.*/
void remove_todos(Celula *ini, int x) {
    Celula *atual = ini->proximo;  // Começa após o nó cabeça
    Celula *anterior = ini;     // Mantém o nó anterior ao atual

    while (atual != NULL) {
        if (atual->valor == x) {
            // Remove o nó atual
            anterior->proximo = atual->proximo;
            free(atual);        // Libera a memória do nó removido
            atual = anterior->proximo;  // Move para o próximo nó
        } else {
            // Avança na lista
            anterior = atual;
            atual = atual->proximo;
        }
    }
}

/* Escreva uma função
void tira_repeticao(Celula *ini);
que remove todos os elementos repetidos da lista encadeada ini, deixando apenas uma cópia de cada um. 
Suponha que a lista tem cabeça. Utilize a função do exercício anterior mesmo que você não a tenha feito. */
void tira_repeticao(Celula *ini) {
    Celula *atual = ini->proximo;  // Começa após o nó cabeça

    while (atual != NULL) {
        // Remove todas as ocorrências do valor de 'atual' após o primeiro
        remove_todos(atual, atual->valor);
        atual = atual->proximo;  // Avança para o próximo nó
    }
}

/* Um conjunto de números inteiros pode ser armazenado em uma lista encadeada. 
Por representarem conjuntos, tais listas não têm elementos repetidos. Escreva uma função com protótipo
Celula *interseccao (Celula *ini1, Celula *ini2);
que recebe duas listas encadeadas ini1 e ini2, sem cabeça de lista, cada uma contendo um conjunto de números inteiros, 
e devolve uma nova lista encadeada sem cabeça, com a interseção dos dois conjuntos. 
Você não deve destruir as listas dadas. Você deve alocar novas células para compor a lista resultante. */
Celula *interseccao(Celula *ini1, Celula *ini2) {
    Celula *resultado = NULL, *ultimo = NULL;
    
    for (Celula *p1 = ini1; p1 != NULL; p1 = p1->proximo) {
        for (Celula *p2 = ini2; p2 != NULL; p2 = p2->proximo) {
            if (p1->valor == p2->valor) {
                Celula *nova = (Celula *)malloc(sizeof(Celula));
                nova->valor = p1->valor;
                nova->proximo = NULL;
                
                if (resultado == NULL) {
                    resultado = nova;
                } else {
                    ultimo->proximo = nova;
                }
                ultimo = nova;
                break;
            }
        }
    }
    
    return resultado;
}

/* Escreva uma função com protótipo
Celula *uniao (Celula *ini1, Celula *ini2);
que recebe duas listas encadeadas ini1 e ini2, sem cabeça de lista, cada uma contendo um conjunto de números inteiros, 
e devolve uma nova lista encadeada sem cabeça, com a união dos dois conjuntos. 
(Não se esqueça que sua lista devolvida não deve ter repetições, para representar corretamente um conjunto.)
Você não deve destruir as listas dadas. Você deve alocar novas células para compor a lista resultante. */
Celula *uniao(Celula *ini1, Celula *ini2) {
    Celula *resultado = NULL, *ultimo = NULL;
    
    // Adiciona todos os elementos de ini1 à lista de resultado
    for (Celula *p1 = ini1; p1 != NULL; p1 = p1->proximo) {
        Celula *nova = (Celula *)malloc(sizeof(Celula));
        nova->valor = p1->valor;
        nova->proximo = NULL;
        
        if (resultado == NULL) {
            resultado = nova;
        } else {
            ultimo->proximo = nova;
        }
        ultimo = nova;
    }

    // Adiciona elementos de ini2 que não estão em ini1
    for (Celula *p2 = ini2; p2 != NULL; p2 = p2->proximo) {
        int ja_existe = 0;
        for (Celula *p1 = ini1; p1 != NULL; p1 = p1->proximo) {
            if (p1->valor == p2->valor) {
                ja_existe = 1;
                break;
            }
        }
        if (!ja_existe) {
            Celula *nova = (Celula *)malloc(sizeof(Celula));
            nova->valor = p2->valor;
            nova->proximo = NULL;
            
            if (resultado == NULL) {
                resultado = nova;
            } else {
                ultimo->proximo = nova;
            }
            ultimo = nova;
        }
    }

    return resultado;
}

/* Refaça o exercício acima supondo que as listas dadas estejam ordenadas em ordem crescente. 
A lista devolvida agora deve também estar ordenada. */
Celula *uniao_ordenada(Celula *ini1, Celula *ini2) {
    Celula *resultado = NULL, *ultimo = NULL;
    
    Celula *p1 = ini1, *p2 = ini2;
    
    while (p1 != NULL || p2 != NULL) {
        Celula *nova = (Celula *)malloc(sizeof(Celula));
        nova->proximo = NULL;
        
        if (p1 == NULL) {
            nova->valor = p2->valor;
            p2 = p2->proximo;
        } else if (p2 == NULL) {
            nova->valor = p1->valor;
            p1 = p1->proximo;
        } else if (p1->valor < p2->valor) {
            nova->valor = p1->valor;
            p1 = p1->proximo;
        } else if (p2->valor < p1->valor) {
            nova->valor = p2->valor;
            p2 = p2->proximo;
        } else {  // Se forem iguais, adiciona um e avança ambos
            nova->valor = p1->valor;
            p1 = p1->proximo;
            p2 = p2->proximo;
        }
        
        if (resultado == NULL) {
            resultado = nova;
        } else {
            ultimo->proximo = nova;
        }
        ultimo = nova;
    }
    
    return resultado;
}

/* Escreva uma função com protótipo
void ordena (Celula *ini);
que recebe uma lista encadeada ini com inteiros e rearraja essa lista de forma que ela fique em ordem crescente, 
devolvendo o início da lista resultante. Suponha que a lista tem cabeça. Não troque o conteúdo das células. 
Apenas altere, quando necessário, os apontadores para ordenar a lista. */
void ordena(Celula *ini) {
    if (ini->proximo == NULL) return; // Lista vazia ou com um elemento

    Celula *atual, *proximo, *menor, *anteriorMenor, *anteriorAtual;

    for (anteriorAtual = ini, atual = ini->proximo; atual != NULL; anteriorAtual = atual, atual = atual->proximo) {
        menor = atual;
        anteriorMenor = anteriorAtual;
        
        for (Celula *p = atual, *ap = anteriorAtual; p != NULL; ap = p, p = p->proximo) {
            if (p->valor < menor->valor) {
                menor = p;
                anteriorMenor = ap;
            }
        }
        
        if (menor != atual) {
            // Troca os nós ao invés dos valores
            anteriorMenor->proximo = menor->proximo;
            menor->proximo = atual->proximo;
            anteriorAtual->proximo = menor;
        }
    }
}


/* PILHAS */

/*Considere uma sequência de elementos entre {, }, [, ], ( e ). Dizemos que uma tal sequência é bem formada se
ela é vazia ou
ela é {S}R ou [S]R ou (S)R, onde S e R são sequências bem formadas.
Escreva uma função que recebe como parâmetros um inteiro n e string com uma sequência de n elementos entre {, }, [, ], ( e ), e devolve 1 se tal sequência é bem formada, 0 caso contrário.
Exemplos: {[()[]]()}({}) é bem formada, enquanto que {[}], {()}[(), (}{) e {()}] não são bem formadas.*/
int bem_formada(int n, char *seq) {
    char pilha[100];
    int topo = -1;

    for (int i = 0; i < n; i++) {
        if (seq[i] == '{' || seq[i] == '[' || seq[i] == '(') {
            pilha[topo++] = seq[i]; // Empilha
        } else {
            char topo_pilha = pilha[--topo]; // Desempilha

            if ((seq[i] == '}' && topo_pilha != '{') ||
                (seq[i] == ']' && topo_pilha != '[') ||
                (seq[i] == ')' && topo_pilha != '(')) {
                return 0; 
            }
        }
    }
    return (topo == -1);
}

/*Reescreva a sua função acima para que ela use a biblioteca "stack.h" vista em aula.*/
int bem_formada_stack(int n, char *seq) {
    Stack pilha = stackInit(n);

    for (int i = 0; i < n; i++) {
        if (seq[i] == '{' || seq[i] == '[' || seq[i] == '(') {
            stackPush(pilha, seq[i]);
        } else {
            if (stackEmpty(pilha)) return 0; 

            char topo_pilha = stackPop(pilha); 

            if ((seq[i] == '}' && topo_pilha != '{') ||
                (seq[i] == ']' && topo_pilha != '[') ||
                (seq[i] == ')' && topo_pilha != '(')) {
                return 0; 
            }
        }
    }

    return stackEmpty(pilha);
}


/*Converta as seguintes expressões para notação posfixa usando o algoritmo visto em aula. Mostre em cada passo o conteúdo da pilha de operadores.
A - B / C * (D - (E + F)) / G + H * I = 
( A + B ) / C * D - E / (F * G)*/


/*Considere expressões que, além dos operadores binários +, -, * e /, têm também o operador binário ^, de exponenciação. O operador ^ tem precedência maior que os operadores acima. Ademais, o operador de exponenciação, diferente dos demais, tem prioridade da direita para a esquerda. Ou seja, a expressão 2^3^2 = 2^(3^2) = 2^9 = 512, e não 64. Adapte o algoritmo visto em aula para aceitar também expressões com ^. Depois simule o seu algoritmo com a seguinte expressão:
A ^ B ^ C + D
A ^ B + C ^ D
A ^ B + C * D ^ E ^ F - G / H ^ (I + J)*/
int precedencia(char operador) {
    if (operador == '^') return 3; // Maior prioridade
    if (operador == '*' || operador == '/') return 2;
    if (operador == '+' || operador == '-') return 1;
    return 0;
}

int associatividade(char operador) {
    if (operador == '^') return 1; // Associativo à direita
    return 0; // Associativo à esquerda
}

void converte_posfixa(char *infixa, char *posfixa) {
    Stack operadores = stackInit(strlen(infixa));
    int k = 0;

    for (int i = 0; infixa[i] != '\0'; i++) {
        if (isalnum(infixa[i])) {
            posfixa[k++] = infixa[i]; // Operando vai direto para a saída
        } else if (infixa[i] == '(') {
            stackPush(operadores, infixa[i]);
        } else if (infixa[i] == ')') {
            while (!stackEmpty(operadores) && stackTop(operadores) != '(') {
                posfixa[k++] = stackPop(operadores);
            }
            stackPop(operadores); // Remove o '('
        } else {
            while (!stackEmpty(operadores) && precedencia(stackTop(operadores)) >= precedencia(infixa[i]) &&
                   (associatividade(infixa[i]) == 0 || associatividade(infixa[i]) == 1)) {
                posfixa[k++] = stackPop(operadores);
            }
            stackPush(operadores, infixa[i]);
        }
    }

    while (!stackEmpty(operadores)) {
        posfixa[k++] = stackPop(operadores);
    }

    posfixa[k] = '\0';
}


/*Escreva uma implementação da biblioteca "stack.h" vista em aula usando lista encadeada sem cabeça.*/
typedef struct stackNode* Link;
struct stackNode { 
  Item conteudo; 
  Link prox; 
};

static Link topo;


void 
stackInit()
{ 
  topo = (Link) mallocSafe(sizeof *topo);
  topo->prox = NULL;
}

int 
stackEmpty()
{ 
  return topo->prox == NULL; 
}

void 
stackPush(Item item)
{ 
  Link nova = (Link) mallocSafe(sizeof *nova);
  nova->conteudo = item;
  nova->prox = topo->prox;
  topo->prox = nova;
}

Item 
stackPop()
{ 
  Link p = topo->prox;
  Item item;

  if (p == NULL) /* stackempty() */
    {
      fprintf(stderr,"Putz, voce nao sabe o que esta fazendo!\n");
      exit(-1);
    }
  /* tudo bem, a pilha nao esta vazia... */
  item = p->conteudo;
  topo->prox = p->prox;
  free(p);
  return item;  
}

Item
stackTop()
{
  if (topo->prox == NULL) /* stackempty() */
    {
      fprintf(stderr,"Putz, voce nao sabe o que esta fazendo!\n");
      exit(-1);
    }

  /* tudo bem, a pilha nao esta vazia... */
  return  topo->prox->conteudo;
}

void 
stackFree()
{
  while (topo != NULL) 
    {
      Link p = topo;
      topo = topo->prox;
      free(p);
    }
}


void
stackDump()
{
  Link p = topo->prox;
  fprintf(stdout, "pilha:");
  if (p == NULL) fprintf(stdout, "vazia.");
  while (p != NULL)
    {
      fprintf(stdout," %c", p->conteudo);
      p = p->prox;
    }
  fprintf(stdout,"\n");
}

/*Considere dada uma função float valor(char ch); que recebe uma letra e devolve o valor da variável dada pela letra. Escreva uma função
float calcula(char *posfixa); que recebe uma expressão posfixa como as calculadas pelo programa que vimos em aula e devolve o valor da expressão. Utilize a função acima para saber o valor das variáveis da expressão. Note que uma pilha vai o ajudar nesse processo. Defina o tipo Item adequadamente e escreva a sua função usando a biblioteca "stack.h" vista em aula.*/
float valor(char ch);

float calcula(char *posfixa) {
    stackInit();
    float op1, op2, resultado;

    for (int i = 0; posfixa[i] != '\0'; i++) {
        char ch = posfixa[i];
        
        if (ch >= 'A' && ch <= 'Z') {
            stackPush(valor(ch));
        } else {
            // Pop the top two operands from the stack
            op2 = stackPop();
            op1 = stackPop();

            // Perform the operation based on the operator
            switch (ch) {
                case '+': resultado = op1 + op2; break;
                case '-': resultado = op1 - op2; break;
                case '*': resultado = op1 * op2; break;
                case '/': resultado = op1 / op2; break;
                case '^': resultado = pow(op1, op2); break;
                default:
                    fprintf(stderr, "Invalid operator: %c\n", ch);
                    exit(EXIT_FAILURE);
            }
            // Push the result back onto the stack
            stackPush(resultado);
        }
    }

    // The final result is the only element left on the stack
    return stackPop();
}


/* FILAS */

/*1. Leia as notas de aula de filas. Entenda o problema e o algoritmo lá explicado para cálculo da distância. Modifique-o para que a saída seja a lista de cidades a que posso chegar a partir da cidade s.*/
/*Problema da aula: 
Considere n cidades numeradas de 0 a n-1
interligadas por estradas de mão única. O comprimento de um caminho é o número
de estradas no caminho, contando-se as repetições. A distância de uma cidade c a uma cidade i é
o menor comprimento de um caminho de c a i.
Se não existe caminho de c a i, a distância é “infinita”. Problema: dada uma rede de estradas e uma cidade c,
determinar a distância de c a cada uma das demais
cidades.*/
/*Algoritmo da aula:
A função distancias recebe um inteiro n, uma matriz
rede representando uma rede de estradas entre n
cidades e uma cidade c e devolve um vetor d que
registra a distância da cidade c a cada uma das outras:
d[i] é a distância de c a i.*/
int *distancias(int n, int **rede, int c)
{
    int *q;  /* guarda a fila */
    int ini; /* q[ini] = 1o. */
    int fim; /* q[fim-1] = ultimo */
    int *d;  /* d[i] = distancia de c a i */
    int j;   /* queueInit(n): inicialize a fila */
    q = mallocSafe(n * sizeof(int));
    ini = 0;
    fim = 0; /* fila vazia */
    /* aloque vetor de distancias */
    d = mallocSafe(n * sizeof(int));
    /* inicialize o vetor de distancias */
    for (j = 0; j < n; j++)
        d[j] = n; /* distancia n = infinito */
    d[c] = 0;
    /* queuePut(c): coloque c na fila */
    q[fim++] = c;
    while (ini != fim)
    {                     /*!queueEmpty()*/
        int i = q[ini++]; /* i = queueGet() */
        int di = d[i];
        for (j = 0; j < n; j++)
            if (rede[i][j] == 1 && d[j] > di + 1)
            {
                d[j] = di + 1;
                q[fim++] = j; /* queuePut(j) */
            }
    }
    free(q); /* queueFree() */
    return d;
}

/*Resposta:*/
int *alcançaveis(int n, int **rede, int c, int *tamanho) {
    int *q;  /* guarda a fila */
    int ini; /* q[ini] = primeiro */
    int fim; /* q[fim-1] = último */
    int *visitado; /* vetor para verificar cidades visitadas */
    int j;  /* contador */
    int *resultado;  /* vetor que guardará as cidades alcançáveis */
    int tam_res = 0; /* tamanho atual do vetor de resultados */

    /* Inicialize a fila */
    q = mallocSafe(n * sizeof(int));
    ini = 0;
    fim = 0; /* fila vazia */

    /* Inicialize o vetor de cidades visitadas */
    visitado = mallocSafe(n * sizeof(int));

    /* Inicialize o vetor de cidades alcançáveis */
    resultado = mallocSafe(n * sizeof(int)); // Inicialmente, pode armazenar todas as cidades

    /* Inicialize o vetor de visitados */
    for (j = 0; j < n; j++)
        visitado[j] = 0;  /* Todas as cidades começam como não visitadas */

    /* Marque a cidade c como visitada e coloque-a na fila */
    visitado[c] = 1;
    q[fim++] = c; /* queuePut(c) */

    /* Enquanto a fila não estiver vazia */
    while (ini != fim) {  /* !queueEmpty() */
        int i = q[ini++]; /* i = queueGet() */

        /* Para cada cidade adjacente à cidade i */
        for (j = 0; j < n; j++) {
            if (rede[i][j] == 1 && !visitado[j]) {
                visitado[j] = 1;   /* Marque como visitada */
                q[fim++] = j;      /* Coloque na fila */

                /* Adicione a cidade j ao resultado */
                resultado[tam_res++] = j;
            }
        }
    }

    free(q);       /* Libere a fila */
    free(visitado); /* Libere o vetor de visitados */

    /* Ajuste o tamanho da lista de cidades alcançáveis */
    resultado = realloc(resultado, tam_res * sizeof(int));
    *tamanho = tam_res; /* Retorne o tamanho do vetor de alcançáveis */

    return resultado;
}

/*2. Considere um grafo G com n vértices, dado por uma matriz anxn em que a[i][j]=1 se os vértices i e j são adjacentes, 0 caso contrário. Dizemos que G é bipartido se podemos colorir os vértices de G com duas cores de modo que toda aresta de G tenha extremos de cores distintas. Escreva uma função que recebe como parâmetros um inteiro n e uma matriz anxn como acima, e devolve 1 se o grafo correspondente for bipartido, 0 caso contrário.*/
/* Função que verifica se um grafo é bipartido */
int ehBipartido(int n, int **a) {
    int *cor = mallocSafe(n * sizeof(int));  // Vetor de cores: 0 = não colorido, 1 = cor 1 e -1 = cor 2
    int *q = mallocSafe(n * sizeof(int));    // Fila para BFS
    int ini = 0, fim = 0;                    // Inicialização da fila

    for (int i = 0; i < n; i++) {
        cor[i] = 0; // Inicializa todos os vértices como não coloridos
    }

    for (int i = 0; i < n; i++) {
        if (cor[i] == 0) {  // Se o vértice não foi colorido
            cor[i] = 1;     // Atribui a primeira cor
            q[fim++] = i;   // Coloca o vértice na fila

            while (ini != fim) {
                int v = q[ini++];  // Desenfileira

                for (int j = 0; j < n; j++) {
                    if (a[v][j] == 1) {  // Se há uma aresta entre v e j
                        if (cor[j] == 0) {  // Se o vértice j ainda não foi colorido
                            cor[j] = -cor[v];  // Atribui a cor oposta ao vértice j
                            q[fim++] = j;      // Coloca j na fila
                        } else if (cor[j] == cor[v]) {  // Se os vértices adjacentes têm a mesma cor
                            free(cor);
                            free(q);
                            return 0;  // O grafo não é bipartido
                        }
                    }
                }
            }
        }
    }

    free(cor);
    free(q);
    return 1;  // O grafo é bipartido
}

/*3. Qual é a saída da seguinte seqüência de operações sobre uma fila:
   queueInit(10);
   queuePut(1);
   queuePut(2);
   printf("%d", queueGet());
   queuePut(3); 
   printf("%d", queueGet());
   printf("%d", queueGet());
   queuePut(4);
   queuePut(5);
   printf("%d", queueGet());
   printf("%d", queueGet());*/
/*Resposta:*/
/*
queueInit(10);: Inicializa a fila com capacidade para 10 elementos.
queuePut(1);: Insere o elemento 1 na fila. Fila: [1]
queuePut(2);: Insere o elemento 2 na fila. Fila: [1, 2]
printf("%d", queueGet());: Remove o primeiro elemento da fila, que é 1, e o imprime. Fila: [2]
Saída até agora: 1
queuePut(3);: Insere o elemento 3 na fila. Fila: [2, 3]
printf("%d", queueGet());: Remove o primeiro elemento da fila, que é 2, e o imprime. Fila: [3]
Saída até agora: 1 2
printf("%d", queueGet());: Remove o primeiro elemento da fila, que é 3, e o imprime. Fila: [] (vazia)
Saída até agora: 1 2 3
queuePut(4);: Insere o elemento 4 na fila. Fila: [4]
queuePut(5);: Insere o elemento 5 na fila. Fila: [4, 5]
printf("%d", queueGet());: Remove o primeiro elemento da fila, que é 4, e o imprime. Fila: [5]
Saída até agora: 1 2 3 4
printf("%d", queueGet());: Remove o primeiro elemento da fila, que é 5, e o imprime. Fila: [] (vazia)
Saída final: 1 2 3 4 5
*/


/*4. Uma fila dupla é uma variante da fila em que se permite inserções nas duas extremidades. Ou seja, há duas operações de inserção numa fila dupla: queuePutL e queuePutR. A remoção é sempre feita da extremidade esquerda. Qual seria o resultado das seguintes operações sobre uma fila dupla:
   dequeInit(10);
   dequePutL(1);
   dequePutR(2);
   printf("%d", dequeGet());
   dequePutR(3); 
   printf("%d", dequeGet());
   printf("%d", dequeGet());
   dequePutR(4);
   dequePutL(5);
   printf("%d", dequeGet());
   printf("%d", dequeGet()); */
/*Resposta:*/
/*
dequeInit(10);: Inicializa uma fila dupla com capacidade para 10 elementos. A fila está vazia: [].
dequePutL(1);: Insere o elemento 1 na extremidade esquerda. Fila: [1].
dequePutR(2);: Insere o elemento 2 na extremidade direita. Fila: [1, 2].
printf("%d", dequeGet());: Remove o elemento da extremidade esquerda, que é 1, e o imprime. Fila: [2].
Saída até agora: 1
dequePutR(3);: Insere o elemento 3 na extremidade direita. Fila: [2, 3].
printf("%d", dequeGet());: Remove o elemento da extremidade esquerda, que é 2, e o imprime. Fila: [3].
Saída até agora: 1 2
printf("%d", dequeGet());: Remove o elemento da extremidade esquerda, que é 3, e o imprime. Fila: [] (vazia).
Saída até agora: 1 2 3
dequePutR(4);: Insere o elemento 4 na extremidade direita. Fila: [4].
dequePutL(5);: Insere o elemento 5 na extremidade esquerda. Fila: [5, 4].
printf("%d", dequeGet());: Remove o elemento da extremidade esquerda, que é 5, e o imprime. Fila: [4].
Saída até agora: 1 2 3 5
printf("%d", dequeGet());: Remove o elemento da extremidade esquerda, que é 4, e o imprime. Fila: [] (vazia).
Saída final: 1 2 3 5 4
*/
















/*
Lembra do problema do ratinho num labirinto procurando o queijo, em MAC2166? Escreva um programa que resolve o problema do ratinho sem usar a interface de fila, mas usando um vetor (dois, na verdade) diretamente como uma fila, como foi feito no cálculo de distância em um grafo, feito em aula.

Modifique o algoritmo do problema do ratinho para que ele resolva a seguinte variante do problema: determinar se não existe caminho do queijo ao rato, se existe exatamente um caminho ou se existem dois ou mais caminhos do queijo ao rato.

Escreva um programa que recebe um labirinto, como no problema do ratinho, e a posição do rato, e imprime todas as posições do labirinto às quais o rato pode chegar.
*/
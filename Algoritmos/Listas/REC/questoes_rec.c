#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Celula
{
    int numero;
    struct Celula *prox; // Apontador para o próximo nó
} Celula;










/*Escreva uma função com protótipo
void acrescenta (Celula *ini, int delta);
que recebe uma lista encadeada ini de números inteiros e soma delta no conteúdo de cada elemento da lista, removendo elementos cujo resultado da soma seja nulo. 
Suponha que a lista encadeada tem cabeça de lista.*/
void acrescenta(Celula *ini, int delta)
{
    if (ini == NULL || ini->prox == NULL)
        return;

    Celula *atual = ini->prox;
    atual->numero += delta;

    if (atual->numero == 0)
    {
        ini->prox = atual->prox;
        free(atual);
        acrescenta(ini, delta);
    }
    else
    {
        acrescenta(atual, delta);
    }
}

/*acrescenta
lista encadeada
com cabeca
fazemos atual = ini->prox
se pularmos, fazemos ini->prox = atual->prox
*/


/*Um conjunto de números inteiros pode ser armazenado em uma lista encadeada. Por representarem conjuntos, tais listas não têm elementos repetidos.
Escreva uma função com protótipo
Celula *interseccao (Celula *ini1, Celula *ini2);
que recebe duas listas encadeadas ini1 e ini2, sem cabeça de lista, cada uma contendo um conjunto de números inteiros, e devolve uma elemento lista encadeada sem cabeça, com a interseção dos dois conjuntos.
Você não deve destruir as listas dadas. Você deve alocar novas células para compor a lista resultante. */
Celula *interseccao(Celula *ini1, Celula *ini2)
{
    Celula *lista = NULL, *ultimo = NULL;

    for (Celula *ini1 = ini1; ini1 != NULL; ini1 = ini1->prox)
    {
        for (Celula *ini2 = ini2; ini2 != NULL; ini2 = ini2->prox)
        {
            if (ini1->numero == ini2->numero)
            {
                Celula *elemento = (Celula *)malloc(sizeof(Celula));
                elemento->numero = ini1->numero;
                elemento->prox = NULL;

                if (lista == NULL)
                {
                    lista = elemento;
                }
                else
                {
                    ultimo->prox = elemento;
                }
                ultimo = elemento;
            }
        }
    }
}
/*
interseccao
guardamos o ultimo e uma lista
dois loops dentro de si
se for igual o numero
criamos celula
checamos se lista esta vazia, se estiver lista = nova celula
se nao estiver, ultimo->prox = nova celula
terminamos com ultimo = nova celula 
*/


/*Escreva uma função com protótipo
Celula *uniao (Celula *ini1, Celula *ini2);
que recebe duas listas encadeadas ini1 e ini2, sem cabeça de lista, cada uma contendo um conjunto de números inteiros, e devolve uma elemento lista encadeada sem cabeça, com a união dos dois conjuntos.
(Não se esqueça que sua lista devolvida não deve ter repetições, para representar corretamente um conjunto.)
Você não deve destruir as listas dadas. Você deve alocar elementos células para compor a lista resultante.*/
Celula *uniao(Celula *ini1, Celula *ini2)
{
    Celula *lista = NULL;

    // Copiamos o conjunto 1 pra nova lista
    while (ini1 != NULL)
    {
        Celula *novo = (Celula *)malloc(sizeof(Celula));
        novo->numero = ini1->numero;
        novo->prox = NULL;

        if (lista == NULL)
        {
            lista = novo;
        }
        else
        {
            lista->prox = novo;
        }
        lista = lista->prox;
        ini1 = ini1->prox;
    }

    int repetido = 0;
    for (Celula *elemento2 = ini2; elemento2 != NULL; elemento2 = elemento2->prox)
    {
        for (Celula *elemento = lista; elemento != NULL; elemento = elemento->prox)
        {
            if (elemento2->numero == lista->numero)
            {
                repetido = 1;
                break;
            }
        }
        if (!repetido)
        {
            lista->numero = elemento2->numero;
            lista = lista->prox;
        }
    }
}
/*
Uniao nao ordenada
copiamos a lista 1 pra lista, usando um loop que cria celula e faz lista = celula se estiver vazia e lista->prox = celula se nao estiver
definimos repetido = 0
iteramos pela lista
iteramos pela lista 2
se numero = numero2, repetido = 1, break pois nao precisamos checar mais

se finalizar um loop e repetido = 0, lista->numero = numero, lista = lista->prox
*/


/* Refaça o exercício acima supondo que as listas dadas estejam ordenadas em ordem crescente.
A lista devolvida agora deve também estar ordenada. */
Celula *uniao_ordenada(Celula *ini1, Celula *ini2)
{
    Celula *lista = (Celula *)malloc(sizeof(Celula));

    while (ini1 != NULL)
    {
        while (ini2 != NULL)
        {
            if (ini1->numero < ini2->numero)
            {
                lista->numero = ini1->numero;
                ini1 = ini1->prox;
            }
            if (ini1->numero > ini2->numero)
            {
                lista->numero = ini2->numero;
                ini2 = ini2->prox;
            }
            else
            {
                lista->numero = ini1->numero;
                ini1 = ini1->prox;
                ini2 = ini2->prox;
            }
        }
    }
}
/*
uniao ordenada

criamos celula lista
iteramos com dois loops
se numero for menor, adicionamos o menor e movemos o indice
se numero for maior, add o menor e move o indice
se numero for igual, add um e move dois indices
*/


/* Escreva uma função com protótipo
void ordena (Celula *ini);
que recebe uma lista encadeada ini com inteiros e rearraja essa lista de forma que ela fique em ordem crescente,
devolvendo o início da lista resultante. Suponha que *a lista tem cabeça*. Não troque o conteúdo das células.
Apenas altere, quando necessário, os apontadores para ordenar a lista. */
void ordena(Celula *ini)
{
    if (ini == NULL || ini->prox->prox == NULL) //lista vazia ou so falta 1 (cabeca)
        return;


    // Cria uma nova lista ordenada que começa com a cabeça e termina em null
    Celula *listaOrdenada = ini;
    listaOrdenada->prox = NULL;
    
    Celula *cabeca = ini;
    // Ponteiro para o restante da lista a ser ordenada
    Celula *atual = ini->prox;
    

    while (atual != NULL)
    {
        // próximo elemento a ser inserido
        Celula *proximo = atual->prox;
        
        // Encontra a posição correta na lista ordenada
        Celula *indice1 = cabeca;
        Celula *indice2 = cabeca->prox;
        
        // Procura onde inserir o elemento atual
        while (indice2 != NULL && indice2->numero < atual->numero)
        {
            indice1 = indice2; //move os dois indices pra frente enquanto indice 2 for menor que o numero do atual, indice2 = cabeca->prox, atual = ini->prox
            indice2 = indice2->prox;
        }
        
        // Insere o elemento na posição correta
        indice1->prox = atual;
        atual->prox = indice2;
        
        // Avança para o próximo elemento
        atual = proximo;
    }
}
/*
ordena

*/









/*Considere n cidades numeradas de 0 a n-1
interligadas por estradas de mão única. O comprimento de um caminho é o número
de estradas no caminho, contando-se as repetições. A distância de uma cidade c a uma cidade i é
o menor comprimento de um caminho de c a i.
Se não existe caminho de c a i, a distância é “infinita”. Problema: dada uma rede de estradas e uma cidade c,
determinar a distância de c a cada uma das demais
cidades.*/
/*A função distancias recebe um inteiro n, uma matriz
rede representando uma rede de estradas entre n
cidades e uma cidade c e devolve um vetor d que
registra a distância da cidade c a cada uma das outras:
d[i] é a distância de c a i.*/
int *distancias(int n, int **rede, int c)
{
    int *q;      /* guarda a fila */
    int ini = 0; /* q[ini] = 1o. */
    int fim = 0; /* q[fim-1] = ultimo */
    int *d;      /* d[i] = distancia de c a i */

    q = (int *)malloc(n * sizeof(int));

    /* aloque vetor de distancias */
    d = (int *)malloc(n * sizeof(int));
    int j;
    /* inicialize o vetor de distancias */
    for (int j = 0; j < n; j++)
        d[j] = n; /* distancia n = infinito */

    d[c] = 0;

    /* queuePut(c): coloque c na fila */
    q[fim++] = c;
    while (ini != fim)
    {
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


int *distancias(int n, int **rede, int c)
{
    int *d; /* d[i] = distancia de c a i*/
    int j;
    queueInit(n); /* inicialize a fila */
    /* aloque vetor de distancias */
    d = mallocSafe(n * sizeof(int));
    /* inicialize o vetor de distancias */
    for (j = 0; j < n; j++)
        d[j] = n; /* distancia n = infinito */
    d[c] = 0;
    queuePut(c); /* coloque c na fila */
    while (!queueEmpty())
    {
        int i = queueGet();
        int di = d[i];
        for (j = 0; j < n; j++)
            if (rede[i][j] == 1 && d[j] > di + 1)
            {
                d[j] = di + 1;
                queuePut(j);
            }
    }
    queueFree();
    return d;
}


/*1. Modifique-o para que a saída seja a lista de cidades a que posso chegar a partir da cidade s.*/
int *alcancaveis(int n, int **rede, int c)
{
    int *q;      /* guarda a fila */
    int ini = 0; /* q[ini] = 1o. */
    int fim = 0; /* q[fim-1] = ultimo */
    int *cid;

    q = (int *)malloc(n * sizeof(int));
    cid = (int *)malloc(n * sizeof(int));

    /* inicialize o vetor de distancias */
    for (int i = 0; i < n; i++)
        cid[i] = n; /* distancia n = infinito */

    cid[c] = c;

    /* queuePut(c): coloque c na fila */
    q[fim++] = c;
    int indice = 0;
    while (ini != fim)
    {
        int ci = q[ini++]; /* i = queueGet() */
        for (int j = 0; j < n; j++)
            if (rede[ci][j] == 1 && cid[j] == n)
            {
                cid[indice] = j;
                indice++;
                q[fim++] = j; /* queuePut(j) */
            }
    }
    free(q); /* queueFree() */
    return cid;
}


int *alcancaveis2(int n, int **rede, int c)
{
    int *q;      /* guarda a fila */
    int ini = 0; /* q[ini] = 1o. */
    int fim = 0; /* q[fim-1] = ultimo */
    int *d;      /* d[i] = distancia de c a i */

    q = (int *)malloc(n * sizeof(int));

    /* aloque vetor de distancias */
    d = (int *)malloc(n * sizeof(int));
    int j;
    /* inicialize o vetor de distancias */
    for (int j = 0; j < n; j++)
        d[j] = n; /* distancia n = infinito */

    d[c] = 0;

    /* queuePut(c): coloque c na fila */
    q[fim++] = c;
    while (ini != fim)
    {
        int i = q[ini++]; /* i = queueGet() */
        int di = d[i];
        for (j = 0; j < n; j++)
            if (rede[i][j] == 1 && d[j] > di + 1)
            {
                d[j] = di + 1;
                q[fim++] = j; /* queuePut(j) */
            }
    }

    /* Conta as cidades alcançáveis */
    int count = 0;
    for (i = 0; i < n; i++)
    {
        if (d[i] != n)
            count++;
    }

    /* Aloca um vetor só com as cidades alcançáveis */
    int *lista = (int *)malloc(count * sizeof(int));

    int cid = 0;

    for (i = 0; i < n; i++)
    {
        if (d[i] != n)
            lista[cid++] = i;
    }

    free(q);
    free(d);

    return lista;
}


//Distancia minima
int *distancia_min(int n, int **redes, int o, int d){
    int *fila;
    int *dist;
    int *visitadas;
    int ini = 0;
    int fim = 0;

    visitadas = mallocSafe(n * sizeof(int));
    fila = mallocSafe(n * sizeof(int));
    dist = mallocSafe(n * sizeof(int));

    for (int i = 0; i < n; i++)
        dist[i] = -1;

    dist[o] = 0;
    visitadas[o] = 1;
    fila[fim++] = o;
    int cid;

    while(ini != fim){
        cid = fila[ini++];

        for (int j = 0; j < n; j++){
            if(redes[cid][j] == 1 && !visitadas[j]){
                dist[j] = dist[cid] + 1;
                if(j == d){
                    free(fila);
                    free(visitadas);
                    return dist[d]; // se for cidade destino, retornamos sua distancia
                }
                visitadas[j] = 1;
                fila[fim++] = j;
            }
        }
    }
}


/*2. Considere um grafo G com n vértices, dado por uma matriz anxn em que a[i][j]=1 se os vértices i e j são adjacentes, 0 caso contrário.
Dizemos que G é bipartido se podemos colorir os vértices de G com *duas cores* de modo que toda aresta de G tenha extremos de cores distintas.
Escreva uma função que recebe como parâmetros um inteiro n e uma matriz anxn como acima, e devolve 1 se o grafo correspondente for bipartido, 0 caso contrário.*/
int *bipartido(int n, int **anxn){
    int *fila;
    int *cor;
    int ini = 0;
    int fim = 0;

    fila = mallocSafe(n * sizeof(int));
    cor = mallocSafe(n * sizeof(int));

    for (int i = 0; i < n; i++)
        cor[i] = -1;

    cor[0] = 0;
    fila[fim++] = 0;

    while(ini != fim){
        int vert = fila[ini++];
        for (int j = 0; j < n; j++){
            if(anxn[vert][j]){ //se forem adjacebres
                if(cor[j] == -1){
                    cor[j] = 1 - cor[vert]; //se j nao estiver pintado, sua cor sera 1 - cor do vertice (oposto)
                    fila[fim++] = j;
                }
                else if(cor[j] == cor[vert]){ //se cor j for igual a cor do vertice, return 0
                    free(cor);
                    free(fila);
                    return 0;
                }
            }
        }
    }
    free(cor);
    free(fila);
    return 1;
}


//Escreva um programa que recebe um labirinto, como no problema do ratinho, e a posição do rato, e imprime todas as posições do labirinto às quais o rato pode chegar.
int** posicoesAlcancaveis(int **redes, int n, int posicao_x, int posicao_y) {
    int dx[] = {-1, 0, 1};
    int dy[] = {-1, 0, 1};

    int ini = 0;
    int fim = 0;
    int* fila_x = malloc(n * sizeof(int));
    int* fila_y = malloc(n * sizeof(int));
    int **visitadas = malloc(n * sizeof(int));
    
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++)
            visitadas[i][j] = 0;
    }

    visitadas[posicao_x][posicao_y] = 1;
    fila_x[fim] = posicao_x;
    fila_y[fim] = posicao_y;
    fim++;

    while(ini != fim){
        int x = fila_x[ini];
        int y = fila_y[ini];
        ini++;

        // Try all 4 directions
        for (int i = 0; i < 3; i++) {
            int nx = x + dx[i]; //loop tentamos fazer px = x + qualquer uma das 3 direcoes, assim como py
            int ny = y + dy[i];
            // Check if new position is valid
            if (nx >= 0 && nx < n && ny >= 0 && ny < n) { //se px e py forem maiores que 0 e menores que n
                // Check if it's a valid path and not visited yet
                if (redes[nx][ny] == 0 && visitadas[nx][ny] == 0) {
                    visitadas[nx][ny] = 1;
                    fila_x[fim] = nx;
                    fila_y[fim] = ny;
                    fim++;
                }
            }
        }
    }
    free(fila_x);
    free(fila_y);

    for (int i = 0; i < n; i ++){
        for (int j = 0; j < n; j++){
            if(visitadas[i][j] == 1)
                printf("%d", visitadas[i][j]);
        }
    }

    return visitadas;
}









/*Escreva uma função com protótipo void removeHeap (int i, int *n, intv[]); 
que recebe um heap em v[1..*n] e remove o elemento v[i] do heap, 
rearranjando v para que continue um heap.*/
//MIN HEAP, MENOR EM CIMA
void removeHeap(int i, int *n, int v[]) {
    // Step 1: Replace the element to be removed with the last element
    v[i] = v[*n];
    // Step 2: Decrease the heap size
    (*n)--;
    
    // Step 3: If we removed the last element, nothing more to do
    if (i == *n + 1) return;
    
    // Step 4: Restore heap property - we need to find the right position for the element
    int temp;
    int j = i;
    
    // First try to bubble up (if the new element is smaller than its parent)
    while (j > 1 && v[j] < v[j/2]) {
        // Swap with parent
        temp = v[j];
        v[j] = v[j/2];
        v[j/2] = temp;
        j = j/2;
    }
    
    // If element didn't move up, try bubbling down
    if (j == i) {
        while (1) {
            int menor = j;
            int left = 2*j;
            int right = 2*j + 1;
            
            // Find the smallest among node and its children
            if (left <= *n && v[left] < v[menor])
                menor = left;
            
            if (right <= *n && v[right] < v[menor])
                menor = right;
            
            // If j is already the menor, we're done
            if (menor == j)
                break;
            
            // Swap with the smallest child
            temp = v[j];
            v[j] = v[menor];
            v[menor] = temp;
            j = menor;
        }
    }
}









/*
O(log n): Quando o problema pode ser resolvido dividindo a entrada a cada passo 
Busca           	A cada passo corta a árvore pela metade
Inserção/Remoção	Caminho da raiz até folha tem altura log n
Heapify             Caminho da raiz até folha tem altura log n

O(n): Quando todos os elementos precisam ser processados pelo menos uma vez 
Construção de Heap	Heapify ocorre em vários nós, e a soma dos custos é n ao inves de log n
BFS em uma árvore	Precisa visitar todos os nós

Calcular operacao na arvore custa n, visitando cada no uma unica vez
Criar expressao posfixa na arvore custa n, pois visita no de esq, dir e raiz, percorrendo cada no uma vez


ABB: Para cada nó da árvore: Todos os nós à esquerda são menores que o nó. Todos os nós à direita são maiores que o nó.
HEAP: O heap não mantém uma ordem total de seus elementos, apenas garante que a raiz (o maior ou menor elemento, dependendo do tipo) esteja sempre no topo.

Heap: Não é possível imprimir os valores de um heap em ordem crescente em tempo O(n) diretamente. 
A operação de extrair repetidamente o menor elemento leva O(log n) para cada elemento, resultando em um tempo total de O(n log n).
ABB: Sim, é possível imprimir os valores de uma ABB em ordem crescente em tempo O(n) através de um percurso inorder (esquerda, raiz, direita).


Construir uma ABB contendo números e então imprimindo o conteúdo da árvore em ordem crescente. 
Qual é o tempo de execução para este algoritmo de ordenação no pior e no melhor caso?
Melhor Caso: Se os números forem inseridos de forma que a ABB permaneça balanceada, a altura da árvore será O(log n). 
A construção da árvore leva tempo O(log n), e o percurso inorder leva tempo O(n). Portanto, o tempo total é O(n log n).
Pior Caso: Se os números forem inseridos em ordem crescente ou decrescente, a ABB tera altura O(n). 
A construção da árvore leva tempo O(n), e o percurso inorder leva tempo O(n). Portanto, o tempo total é O(n^2).
*/

No* removerNo(No* p) {
    if (p == NULL)
        return NULL;
        
    // Caso 1 e 2: Nó tem zero ou um filho
    if (p->esq == NULL) {
        No* temp = p->dir;
        free(p);
        return temp;
    }
    if (p->dir == NULL) {
        No* temp = p->esq;
        free(p);
        return temp;
    }
    
    // Caso 3: Nó tem dois filhos
    // Encontra o sucessor inorder (menor nó da subárvore direita)
    No* sucessor = p->dir;
    while (sucessor->esq != NULL)
        sucessor = sucessor->esq;
    
    // Copia o valor do sucessor
    p->valor = sucessor->valor;
    
    // Remove recursivamente o sucessor da subárvore direita
    p->dir = removerNo(p->dir);
    
    return p;
}











/*
Problema das N Rainhas
Descrição do Problema
O problema das N Rainhas consiste em colocar N rainhas em um tabuleiro de xadrez NxN de modo que nenhuma rainha ataque outra. Uma rainha pode atacar outra se elas estiverem na mesma linha, coluna ou diagonal. A tarefa é encontrar todas as formas possíveis de posicionar as N rainhas de forma que nenhuma delas possa se atacar.

Solução Usando Backtracking
O algoritmo de backtracking tenta colocar uma rainha em cada linha, tentando todas as posições possíveis em cada coluna, e recursivamente tentando posicionar as próximas rainhas nas linhas seguintes, garantindo que nenhuma rainha possa atacar outra.

Passos principais:

Tente colocar uma rainha na primeira linha em todas as colunas.
Para cada tentativa, verifique se a rainha está em uma posição segura (não na mesma linha, coluna ou diagonal que as outras).
Se a posição for segura, passe para a próxima linha e continue tentando.
Se uma posição for inválida, volte (backtrack) e tente a próxima posição possível para a rainha na linha indice1.
*/
void nRainhas(int n)
{
    int i;             /* linha atual */
    int j;             /* coluna candidata */
    int nJogadas = 0;  /* num. da jogada */
    int nSolucoes = 0; /* num. de solucoes */
    int *s = mallocSafe((n + 1) * sizeof(int));
    /* s[i] = coluna da linha i em que
    * esta’ a rainha i, para i = 1,...,n.
    * Posicao s[0] nao sera usada.
    /* linha inicial e coluna inicial */
    i = j = 1;
    /* Encontra todas as solucoes. */
    while (i > 0)
    {
        /* s[1 . . i-1] e’ solucao parcial */
        int achouPos = 0;
        while (j <= n && !achouPos)
        {
            s[i] = j;
            nJogadas += 1;
            if (solucaoParcial(i, s))
                achouPos = 1;
            else
                j += 1;
        }
        if (achouPos)
        { /* AVANCA */
            i += 1;
            j = 1;
            if (i == n + 1)
            {
                /* uma solucao foi encontrada */
                nSolucoes++;
                mostreTabuleiro(n, s);
                j = s[--i] + 1; /* volta */
            }
        }
        else
        { /* BACKTRACKING */
            j = s[--i] + 1;
        }
    }
    printf(stdout, "\n no. jogadas = %d"
                   "\n no. solucoes = %d.\n\n",
           nJogadas, nSolucoes);
    free(s);
}


/*
Dado um conjunto de números inteiros, determinar se ele pode ser dividido em dois subconjuntos cuja soma seja igual.

Calcular a soma total do conjunto percorrendo o array com um for.
Se a soma for ímpar, já podemos retornar 0 (falso), pois não dá para dividir um número ímpar em duas partes inteiras iguais.
Definir target = total_sum / 2 e usar backtracking para verificar se existe um subconjunto que some exatamente target.
Implementar a recursão, tentando incluir ou não cada elemento no subconjunto atual.
*/
// Função recursiva para verificar se existe um subconjunto com soma igual a target
int backtrack(int nums[], int n, int index, int current_sum, int target) {
    if (current_sum == target) {
        return 1;  // Se encontramos um subconjunto cuja soma seja exatamente igual ao target, retornamos verdadeiro (1)
    }
    if (current_sum > target || index >= n) {
        return 0;  // Se ultrapassamos o target ou não há mais elementos para processar, retornamos falso (0)
    }

    // Tentar incluir nums[index] no subconjunto OU ignorá-lo
    return backtrack(nums, n, index + 1, current_sum + nums[index], target) || 
           backtrack(nums, n, index + 1, current_sum, target);
}

// Função principal para verificar se o conjunto pode ser particionado em dois subconjuntos de soma igual
int can_partition(int nums[], int n) {
    int total_sum = 0;

    // Calcula a soma total do conjunto
    for (int i = 0; i < n; i++) {
        total_sum += nums[i];
    }

    // Se a soma total for ímpar, não podemos dividir em dois subconjuntos iguais
    if (total_sum % 2 != 0) {
        return 0;
    }

    // Definir o target como metade da soma total
    int target = total_sum / 2;

    // Chamar a função de backtracking
    return backtrack(nums, n, 0, 0, target);
}
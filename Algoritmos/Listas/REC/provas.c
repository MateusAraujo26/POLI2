/*Escreva uma função recursiva com protótipo
   int maximo (int v[], int ini, int fim);
que devolve o maior valor do vetor entre as posições ini e fim. 
Use uma estratégia como a do mergesort (divisão e conquista): divida o vetor ao meio, 
chame a função recursivamente para cada uma das metades, depois "junte" as respostas.*/
int maximo (int v[], int ini, int fim){
   if(ini == fim)
      return v[ini];
   int meio = (ini + fim) / 2;

   int max1 = maximo(v, ini, meio);
   int max2 = maximo(v, meio + 1, fim);

   return (max1 > max2) ? max1 : max2;
}


/*Escreva uma função recursiva busca_ternária com o seguinte protótipo
   int busca_ternaria 
   (int v, int ini, int fim, int x);
Sua função deve devolver 1 se x aparece no vetor, 0 caso contrário. Inspire-se na busca binária. 
Na busca ternária, você deve dividir o vetor em três (em vez de em dois), 
comparar o x com os dois elementos separadores dessas três partes e, ou encontra x, 
ou decide em qual das partes ele pode estar, procurando-o recursivamente nesta parte.*/
int busca_ternaria(int v[], int ini, int fim, int x) {
    if (ini > fim) {
        return 0; // Elemento não encontrado
    }

    if (v[ini] == x || v[fim] == x) {
        return 1; // Elemento encontrado

    // Dividir o array em três partes
    int terco1 = ini + (fim - ini) / 3;
    int terco2 = fim - (fim - ini) / 3;

    if (x < v[terco1]) {
        // Buscar no primeiro terço
        return busca_ternaria(v, ini, terco1 - 1, x);
    } else if (x > v[terco2]) {
        // Buscar no terceiro terço
        return busca_ternaria(v, terco2 + 1, fim, x);
    } else {
        // Buscar no segundo terço
        return busca_ternaria(v, terco1 + 1, terco2 - 1, x);
    }
}


/*Escreva uma função que recebe um vetor com n letras As e Bs e, por meio de trocas, 
move todos os As para o início do vetor. Sua função deve consumir tempo linear 
(proporcional ao tamanho do vetor, ou seja, a n).*/
void ordena_AB(char vec[], int n) {
    int esq = 0;
    int dir = n - 1;

    while (esq < dir) {
        // Encontra o primeiro 'A' a partir da esq
        while (esq < dir && vec[esq] == 'A') {
            esq++;
        }
        // Encontra o primeiro 'B' a partir da dir
        while (esq < dir && vec[dir] == 'B') {
            dir--;
        }
        // Troca os elementos se necessário
        if (esq < dir) {
            v[esq] = A;
            v[dir] = B;
        }
    }
}


/*Escreva uma função com protótipo
      void alteraChave (int x, int i, int n, intv[]); 
que recebe um heap em v[1..n] e altera a chave do elemento i para x, 
rearranjando v para que continue um heap */
void alteraChave(int x, int i, int n, int v[]) {
    v[i] = x;

    // Subir o elemento no heap (heapify-up)
    while (i > 1 && v[i] > v[i/2]) {
        int temp = v[i / 2];
        v[i / 2] = v[i];
        v[i] = temp;
        i /= 2;
    }
    // Descer elemento
    while (1)
    {
        int pai = i;
        int esq = 2 * i;
        int dir = 2 * i + 1;
        if(esq <= n && v[esq] > v[pai]){
            pai = esq;
        }
        if(dir <= n && v[dir] > v[pai]){
            pai = dir;
        }
        if(pai != i){
            int temp = v[i];
            v[i] = v[pai];
            v[pai] = temp;
            i = pai;
        }
        else{
            break;
        }
    }
}
    


/*Escreva uma função com protótipo
    void removeHeap (int i, int *n, intv[]);
que recebe um heap em v[1..*n] e remove o elemento v[i] do heap, rearranjando v para que continue um heap.*/
void removeHeap (int i, int *n, intv[]){
    // Substituir o elemento a ser removido pelo último elemento do heap
    v[i] = v[*n];
    (*n)--;
    // Heapify down
    while (1) {
        maior = i;
        esquerda = 2 * i;
        direita = 2 * i + 1;

        if (esquerda <= *n && v[esquerda] > v[maior]) {
            maior = esquerda;
        }
        if (direita <= *n && v[direita] > v[maior]) {
            maior = direita;
        }
        if (maior != i) {
            int temp = v[i];
            v[i] = v[maior];
            v[maior] = temp;
            i = maior;
        } else {
            break;
        }
    }
}
}


/*Escreva uma função monta-árvore que recebe uma expressão aritmética com operadores +, -, * e /
e operandos identificados por uma letra cada um, e devolve uma árvore binária que representa essa expressão,
conforme o exemplo mostrado em aula.*/
No* cria_no(char valor) {
    No* no = (No*)malloc(sizeof(No));
    no->valor = valor;
    no->esquerda = no->direita = NULL;
    return no;
}

No* monta_arvore(char *expressao) {
    Stack stack;
    stack_init(&stack);
    
    for (int i = 0; expressao[i] != '\0'; i++) {
        if (isalpha(expressao[i])) {
            No* no = cria_no(expressao[i]);
            stack_push(&stack, no);
        }
        else{
            No* direita = stack_pop(&stack);
            No* esquerda = stack_pop(&stack);
            No* no = cria_no(expressao[i]);
            no->esquerda = esquerda;
            no->direita = direita;
            stack_push(&stack, no);
        }
    }
    return stack_pop(&stack);
}

/*Escreva uma função calcula que recebe a árvore que você montou no exercício 1 e devolve o valor da expressão 
nela guardada, supondo que você tem à sua disposição uma rotina valor que, dada uma letra, devolve o valor 
associado àquela letra na expressão. Quanto tempo sua função consome em função do número de nós da árvore?*/
int calcula(Arvore r)
{
   if (r == NULL)
      return 0;
   int valor_esq = calcula(r->esq);
   int valor_dir = calcula(r->dir);

   switch (r->valor)
   {
   case '+':
      return valor_esq + valor_dir;
   case '-':
      return valor_esq - valor_dir;
   case '*':
      return valor_esq * valor_dir;
   case '/':
      return valor_esq / valor_dir;
   default:
      return valor(r->valor);
   }
}





/*Escreva uma função recursiva máximo que recebe uma árvore r não-vazia e 
devolve um apontador para o nó com conteúdo máximo em r.*/
No* maximo(No* r) {
    if (r == NULL) {
        return NULL;
    }

    No* maxEsquerda = maximo(r->esquerda);
    No* maxDireita = maximo(r->direita);

    No* maxNo = r;
    if (maxEsquerda != NULL && maxEsquerda->valor > maxNo->valor) {
        maxNo = maxEsquerda;
    }
    if (maxDireita != NULL && maxDireita->valor > maxNo->valor) {
        maxNo = maxDireita;
    }

    return maxNo;
}


/*Escreva uma função nível que recebe uma ABB r e um elemento x que aparece na árvore r, e devolve o 
nível do nó que contém x em r.*/
int nivel(No* r, char x) {
    if (r == NULL) {
        return -1; // Elemento não encontrado
    }

    if (r->valor == x) {
        return 0; // Elemento encontrado na raiz
    }

    int nivelSubarvore;
    if (x < r->valor) {
        nivelSubarvore = nivel(r->esquerda, x);
    } else {
        nivelSubarvore = nivel(r->direita, x);
    }

    if (nivelSubarvore == -1) {
        return -1; // Elemento não encontrado nas subárvores
    }

    return nivelSubarvore + 1; // Incrementa o nível ao subir na árvore
}


/*Escreva a função remoção, que recebe o apontador p para um nó de uma ABB e remove o conteúdo do nó 
apontado por p da ABB, deixando-a uma ABB após a operação.*/
No* remocao(No* p) {
    if (p == NULL) {
        return NULL;
    }

    // Caso 1: Nó com um ou nenhum filho
    if (p->esquerda == NULL) {
        No* temp = p->direita;
        free(p);
        return temp;
    } else if (p->direita == NULL) {
        No* temp = p->esquerda;
        free(p);
        return temp;
    }

    // Caso 2: Nó com dois filhos
    // Encontrar o sucessor in-order (baixo  valor na subárvore direita)
    No* temp = p->direita;
    while (temp && temp->esquerda != NULL) {
        temp = temp->esquerda;
    }

    // Copiar o valor do sucessor para o nó atual
    p->valor = temp->valor;

    // Remover o sucessor in-order
    p->direita = remocao(p->direita);

    return p;
}




/*Declare um tipo struct celula, com dois campos: chave (int) e Link (apontador para struct celula). 
Usando esse tipo, escreva uma implementação das funções
   int h(int m, int k);

   int busca(Link T[], int m, int k);

   void insere (Link T[], int m, int k);

   void remove (Link T[], int m, int k); 
em que h é é uma função de hash sugerida na aula e a resolução de colisões é por lista encadeada. 
Para escolher o valor de m e a função de hash, considere uma aplicação em que 
vão ser armazenados na tabela de hashing cerca de 1000 inteiros.*/
typedef struct celula {
    int chave;
    struct celula* prox;
} celula;

typedef celula* Link;

int h(int m, int k) {
    return k % m;
}

int busca(Link T[], int m, int k) {
    int i = h(m, k);
    Link atual = T[i];
    while (atual != NULL) {
        if (atual->chave == k) {
            return 1; // Elemento encontrado
        }
        atual = atual->prox;
    }
    return 0; // Elemento não encontrado
}

void insere(Link T[], int m, int k) {
    int i = h(m, k);
    Link novo = (Link)malloc(sizeof(celula));
    novo->chave = k;
    novo->prox = T[i];
    T[i] = novo;
}

void remove(Link T[], int m, int k) {
    int i = h(m, k);
    Link atual = T[i];
    Link anterior = NULL;

    while (atual != NULL) {
        if (atual->chave == k) {
            if (anterior == NULL) {
                T[i] = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }
            free(atual);
            return;
        }
        anterior = atual;
        atual = atual->prox;
    }
}   
https://www.ime.usp.br/~cris/mac0122/slides/aula22.pdf


/*Calcule os vetores ult e alcance, usados nas heurísticas do algoritmo Boyer-Moore, para o padrão abaixo:
      a    b    b    a    b    a    b    b    a    a    b */

/*Vetor ult
O vetor ult armazena a última ocorrência de cada caractere no padrão. Se um caractere não aparece no padrão, seu valor será -1.
1. Inicialize o vetor ult com -1 para todos os caracteres.
2. Percorra o padrão da esquerda para a direita e atualize o vetor ult com a posição de cada caractere.*/  

/*Vetor alcance
O vetor alcance é usado na heurística de sufixo do algoritmo Boyer-Moore. Ele armazena a maior posição j tal que o sufixo do padrão que começa na posição j é um prefixo do padrão.
1. Inicialize o vetor alcance com zeros.
2. Percorra o padrão da direita para a esquerda e atualize o vetor alcance com a maior posição j tal que o sufixo do padrão que começa na posição j é um prefixo do padrão.*/

/*Resultado final do vetor ult:
ult['a'] = 9
ult['b'] = 10*/

/*Sufixo	Prefixo correspondente	alcance
  a	        a	                    1
  aa	    aa	                    2
O resto eh 0, logo [1, 2, 0, 0...]*/


/*Escreva uma função que tem como parâmetro duas palavras de mesmo comprimento 
e determina se uma é permutação cíclica da outra. Por exemplo, arco e coar são rotações uma da outra.*/
/* Função que verifica se uma palavra é uma permutação cíclica de outra */
int eh_permutacao_ciclica(char palavra1[], char palavra2[]) {
    int len1 = strlen(palavra1);
    int len2 = strlen(palavra2);

    // Cria uma string concatenada com a segunda palavra duas vezes
    char concatenada[2 * len2 + 1];
    strcpy(concatenada, palavra2);
    strcat(concatenada, palavra2);

    // Usa o algoritmo de Boyer-Moore para verificar se a primeira palavra está na string concatenada
    return BoyerMoore(palavra1, len1, concatenada, 2 * len2) > 0;
}


int BoyerMoore(char p[], int m, char t[], int n) {
    int ult[256];
    int i, r, k, ocorrs;
    /* pre-processamento da palavra p */
    for (i = 0; i < 256; i++) ult[i] = 0;
    for (i = 1; i <= m; i++) ult[p[i]] = i;
    /* busca da palavra p no texto t */
    ocorrs = 0; k = m;
    while (k <= n) {
        r = 0;
        while (r < m && p[m - r] == t[k - r])
            r += 1;
        if (r == m) ocorrs += 1;
        if (k == n) k += 1; /* cai fora */
        else k += m - ult[t[k + 1]] + 1;
    }
    return ocorrs;
}
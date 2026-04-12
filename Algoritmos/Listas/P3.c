#include <stdio.h>
#include <stdlib.h>
#include "stack.c"



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

    // Dividir o array em três partes
    int terco1 = ini + (fim - ini) / 3;
    int terco2 = fim - (fim - ini) / 3;

    if (v[terco1] == x ) {
        return 1; // Elemento encontrado

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

/*Notação prefixa é definida de forma semelhante a notação posfixa. 
Na notação prefixa, os operadores aparecem antes de seus operandos. 
Assim, por exemplo, a expressão
   A + B * C - ( D - E / F + G ) / H + I
em notação prefixa ficaria:
   - + A * B C + / + - D / E F G H I
Escreva uma função recursiva que calcula o valor de uma expressão prefixa. Suponha que exista uma função
   int valor (char ch); 
que, dada uma letra, devolve o valor da variável correspondente àquela letra.
*/

/*Simule os algoritmos mergesort, quicksort e heapsort com o seguinte vetor de entrada:
   15   27   3   18   7   11   22   19   9   10   1   5   8   14

Mergesort: divide em pedacos unitarios e sort
15   27   3   18   7   11   22       19   9   10   1   5   8   14
15 27 3   18 7 11 22   19 9 10   1 5 8 14
15    27 3   18   7 11 22   19   9 10   1    5 8 14
separa de um a um, e agora junta ordenando
15    3 27     18    7 11 22     19    9 10    1    5 8 14
3 15 27     7 11 18 22     9 10 19     1 5 8 14
3 7 11 15 18 22 27      1 5 8 9 10 14 19
1 3 5 7 8 9 10 11 14 15 18 19 22 27

Quicksort: escolhe pivo, troca com esquerda e vai olhando numeros depois
pivo 11, substitui com o ultimo. esquerda 15, direita 8
15   27   3   18   7   14   22   19   9   10   1   5   8   11
8   27   3   18   7   14   22   19   9   10   1   5   15   11
8   5   3   18   7   14   22   19   9   10   1   27   15   11
8   5   3   1   7   14   22   19   9   10   18   27   15   11
8   5   3   1   7   10   22   19   9   14   18   27   15   11
8   5   3   1   7   10   9   19   22   14   18   27   15   11
O indice do primeiro maior excedeu o indice do primeiro menor, coloca o 11 no meio do vetor
8   5   3   1   7   10   9   11   19   22   14   18   27   15
Escolhe outro pivo, continua.

Heapsort:

*/

/*Simule a execução do algoritmo quicksort no vetor abaixo:
   17    10    7    23    15    3    1    20    8    4
Indique na sua simulação as comparações que estão sendo feitas e o resultado de cada chamada do separa. 
Quantas chamadas do separa foram feitas neste caso? Quantas auto-chamadas o quicksort (recursivo) para 
o vetor acima geraria?*/

/*Escreva uma função que recebe um vetor com n letras As e Bs e, por meio de trocas, 
move todos os As para o início do vetor. Sua função deve consumir tempo linear 
(proporcional ao tamanho do vetor, ou seja, a n).*/

/*Dê uma lista não ordenada de 10 elementos em que o quicksort se dá o pior possível. 
Dê uma lista de 10 elementos onde o quicksort se dá o melhor possível.*/
/*
10 9 8 7 6 5 4 3 2 1
4 1 3 2 5 10 6 9 7 8
*/


No* removeABB(No* raiz, int x) {
    if (raiz == NULL) {
        // Árvore vazia ou elemento não encontrado
        return NULL;
    }
    if (x < raiz->valor) {
        // Busca na subárvore esquerda
        raiz->esquerda = removeABB(raiz->esquerda, x);
    } else if (x > raiz->valor) {
        // Busca na subárvore direita
        raiz->direita = removeABB(raiz->direita, x);
    } else {
        // Encontrou o nó a ser removido
        raiz = RemoveRaiz(raiz);
    }
    return raiz;
}
/*Escreva uma função com protótipo
      void alteraChave (int x, int i, int n, intv[]); 
que recebe um heap em v[1..n] e altera a chave do elemento i para x, 
rearranjando v para que continue um heap
 * Função para alterar a chave do elemento i para x e rearranjar o heap
 * Parâmetros:
 * - x: novo valor para a chave
 * - i: índice do elemento cuja chave será alterada
 * - n: número de elementos no heap
 * - v: array que representa o heap (indexado de 1 a n)
 */
void alteraChave(int x, int i, int n, Maquina v[]) {
    int original = v[i].carga;
    v[i].carga = x;

    if (x > original) {
        // Se a nova chave é maior, precisamos ajustar o heap para cima
        while (i > 0 && v[(i - 1) / 2].carga > v[i].carga) {
            Maquina temp = v[i];
            v[i] = v[(i - 1) / 2];
            v[(i - 1) / 2] = temp;
            i = (i - 1) / 2;
        }
    } else if (x < original) {
        // Se a nova chave é menor, precisamos ajustar o heap para baixo
        int left, right, smallest;
        while (1) {
            left = 2 * i + 1;
            right = 2 * i + 2;
            smallest = i;

            if (left < n && v[left].carga < v[smallest].carga) {
                smallest = left;
            }
            if (right < n && v[right].carga < v[smallest].carga) {
                smallest = right;
            }
            if (smallest == i) {
                break;
            }

            Maquina temp = v[i];
            v[i] = v[smallest];
            v[smallest] = temp;
            i = smallest;
        }
    }
}

/*Escreva uma função com protótipo
      void removeHeap (int i, int *n, intv[]); 
que recebe um heap em v[1..*n] e remove o elemento v[i] do heap, rearranjando v para que continue um heap.
 * Função para remover o elemento v[i] do heap e rearranjar o heap
 * Parâmetros:
 * - i: índice do elemento a ser removido
 * - n: ponteiro para o número de elementos no heap
 * - v: array que representa o heap (indexado de 0 a *n-1)
 */
void removeHeap(int i, int *n, Maquina v[]) {
    // Troca o elemento a ser removido com o último elemento do heap
    Maquina temp = v[i];
    v[i] = v[*n - 1];
    v[*n - 1] = temp;

    // Reduz o tamanho do heap
    (*n)--;

    // Ajusta o heap para manter a propriedade de heap
    if (i < *n) {
        int left, right, smallest;
        while (1) {
            left = 2 * i + 1;
            right = 2 * i + 2;
            smallest = i;

            if (left < *n && v[left].carga < v[smallest].carga) {
                smallest = left;
            }
            if (right < *n && v[right].carga < v[smallest].carga) {
                smallest = right;
            }
            if (smallest == i) {
                break;
            }

            Maquina temp = v[i];
            v[i] = v[smallest];
            v[smallest] = temp;
            i = smallest;
        }

        // Ajusta o heap para cima, se necessário
        while (i > 0 && v[(i - 1) / 2].carga > v[i].carga) {
            Maquina temp = v[i];
            v[i] = v[(i - 1) / 2];
            v[(i - 1) / 2] = temp;
            i = (i - 1) / 2;
        }
    }
}


/*Escreva uma função monta-árvore que recebe uma expressão aritmética com operadores +, -, * e / 
e operandos identificados por uma letra cada um, e devolve uma árvore binária que representa essa expressão, 
conforme o exemplo mostrado em aula.*/

/*Escreva uma função calcula que recebe a árvore que você montou no exercício 1 e devolve o valor da expressão 
nela guardada, supondo que você tem à sua disposição uma rotina valor que, dada uma letra, devolve o valor 
associado àquela letra na expressão. Quanto tempo sua função consome em função do número de nós da árvore?*/
int calcula(Arvore r)
{
   if (r == NULL)
      return 0;
   int valor_esq = calcula(r->esquerda);
   int valor_dir = calcula(r->direita);

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

/*Escreva uma função numNos que recebe uma árvore r e devolve o número de nós que há na árvore r.*/
int numNos(Arvore r){
   if(r == NULL) return 0;
   return (calcula(r->dir) + calcula r->esq + 1);
}

/*Escreva uma função predecessor que recebe o apontador q para um nó de um ABB e devolve um apontador para 
o nó que precede q na árvore (e NULL caso q seja o mínimo da ABB).*/

/*Escreva uma função nível que recebe uma ABB r e um elemento x que aparece na árvore r, e devolve o 
nível do nó que contém x em r.*/

/*Escreva a função remoção, que recebe o apontador p para um nó de uma ABB e remove o conteúdo do nó 
apontado por p da ABB, deixando-a uma ABB após a operação.*/



/*Declare um tipo struct celula, com dois campos: chave (int) e Link (apontador para struct celula). 
Usando esse tipo, escreva uma implementação das funções
   int h(int m, int k);

   int busca(Link T[], int m, int k);

   void insere (Link T[], int m, int k);

   void remove (Link T[], int m, int k); 
em que h é é uma função de hash sugerida na aula e a resolução de colisões é por lista encadeada. 
Para escolher o valor de m e a função de hash, considere uma aplicação em que 
vão ser armazenados na tabela de hashing cerca de 1000 inteiros.*/

/*Calcule os vetores ult e alcance, usados nas heurísticas do algoritmo Boyer-Moore, para o padrão abaixo:
      a    b    b    a    b    a    b    b    a    a    b */
      
/*Escreva uma função que tem como parâmetro duas palavras de mesmo comprimento 
e determina se uma é permutação cíclica da outra. Por exemplo, arco e coar são rotações uma da outra.*/



/*EPS:*/
void heapify(int indice, int size, Maquina arr[]) {
    int menor = indice;
    int galho_esquerdo = 2 * indice + 1;
    int galho_direito = 2 * indice + 2;

    if (galho_esquerdo < size && arr[galho_esquerdo].carga < arr[menor].carga) {
        menor = galho_esquerdo;
    }

    if (galho_direito < size && arr[galho_direito].carga < arr[menor].carga) {
        menor = galho_direito;
    }

    if (menor != indice) {
        Maquina temp = arr[indice];
        arr[indice] = arr[menor];
        arr[menor] = temp;

        heapify(menor, size, arr);
    }
}

void sortInd(int n, int d[], int id[]) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (d[id[i]] < d[id[j]]) {
                int temp = id[i];
                id[i] = id[j];
                id[j] = temp;
            }
        }
    }
}



/*ALGORITMOS DE ORDENACAO:*/
/* Mergesort:
 * Divide array em 2 partes recursivamente, ate tornar unitario. Entao faz o sort recursivamente juntando o array
 */
void merge(int arr[], int l, int m, int r) {
   int i, j, k;
   int n1 = m - l + 1;
   int n2 = r - m;

   // Cria arrays temporários
   int L[n1], R[n2];

   // Copia os dados para os arrays temporários L[] e R[]
   for (i = 0; i < n1; i++)
      L[i] = arr[l + i];
   for (j = 0; j < n2; j++)
      R[j] = arr[m + 1 + j];

   // Junta os arrays temporários de volta em arr[l..r]
   i = 0; // Índice inicial do primeiro subarray
   j = 0; // Índice inicial do segundo subarray
   k = l; // Índice inicial do subarray mesclado
   while (i < n1 && j < n2) {
      if (L[i] <= R[j]) {
         arr[k] = L[i];
         i++;
      } else {
         arr[k] = R[j];
         j++;
      }
      k++;
   }

   // Copia os elementos restantes de L[], se houver
   while (i < n1) {
      arr[k] = L[i];
      i++;
      k++;
   }

   // Copia os elementos restantes de R[], se houver
   while (j < n2) {
      arr[k] = R[j];
      j++;
      k++;
   }
}

void mergeSort(int arr[], int l, int r) {
   if (l < r) {
      int m = l + (r - l) / 2;

      // Ordena as duas metades
      mergeSort(arr, l, m);
      mergeSort(arr, m + 1, r);

      // Junta as duas metades ordenadas
      merge(arr, l, m, r);
   }
}

/*Quicksort*/
// Função para trocar dois elementos
void swap(int* a, int* b) {
   int temp = *a;
   *a = *b;
   *b = temp;
}

// Função para particionar o array
int partition(int arr[], int low, int high) {
   int pivot = arr[high];
   int i = (low - 1);

   for (int j = low; j <= high - 1; j++) {
      if (arr[j] <= pivot) {
         i++;
         swap(&arr[i], &arr[j]);
      }
   }
   swap(&arr[i + 1], &arr[high]);
   return (i + 1);
}

// Função para implementar QuickSort
void quickSort(int arr[], int low, int high) {
   // Verifica se o índice inicial é menor que o índice final
   if (low < high) {
      // Particiona o array e obtém o índice do pivô
      int pi = partition(arr, low, high);

      // Ordena recursivamente os elementos antes da partição
      quickSort(arr, low, pi - 1);
      // Ordena recursivamente os elementos após a partição
      quickSort(arr, pi + 1, high);
   }
}

/*Heapsort:*/
// Função para ajustar um heap com raiz no nó i
void heapifySort(int arr[], int n, int i) {
   int largest = i;
   int left = 2 * i + 1;
   int right = 2 * i + 2;

   if (left < n && arr[left] > arr[largest])
      largest = left;

   if (right < n && arr[right] > arr[largest])
      largest = right;

   if (largest != i) {
      swap(&arr[i], &arr[largest]);
      heapifySort(arr, n, largest);
   }
}

// Função para implementar HeapSort
void heapSort(int arr[], int n) {
   // Constrói o heap (rearranja o array)
   // Começa do último nó não-folha e aplica heapify para todos os nós acima dele
   for (int i = n / 2 - 1; i >= 0; i--)
      heapify(arr, n, i);

   // Extrai um elemento do heap de cada vez
   // Move o maior elemento (raiz do heap) para o final do array
   // Reduz o tamanho do heap e aplica heapify na nova raiz
   for (int i = n - 1; i > 0; i--) {
      swap(&arr[0], &arr[i]);
      heapify(arr, i, 0);
   }
}



/*EXTRAS*/
/*Escreva uma função recursiva máximo que recebe uma árvore r não-vazia 
e devolve um apontador para o nó com conteúdo máximo em r.*/

/*Escreva uma função espelha que recebe uma árvore r e devolve a árvore espelhada, 
composta pelos mesmo nós que r.*/

/*
Quantos "alinhamentos" do padrão acima são testados no texto abaixo, ou seja, quantas posições diferentes do padrão em relação ao texto geraram pelo menos uma comparação entre o padrão e o texto durante a execução de cada uma das três versões do algoritmo de Boyer-Moore?
      a    b    a    b    b    a    a    b    b    a    b    b    a    b    a    b    b    a    a    b    b 
    
Calcule o vetor alcance para o padrão
  
      a   b   a   b   b   a   b   b   a   b   a   b   b   a   b   a   b   b   a   b   b*/

/* ************************************************************ */
/*           Parte do EP4 de MAC0122-2024: heap.c               */
/*            Implementação de um heap de mínimo                */
/* ************************************************************ */

#include "heap.h"
#include <stdio.h>
#include <stdlib.h>

/* Protótipos das rotinas auxiliares */
void heapify(Maquina v[], int m, int i);
void buildHeap(Maquina v[], int m);
int comparacao(const void *a, const void *b);
void sortInd(int n, int d[], int id[]);
void sort_tasks_desc(int n, int d[], int idx[]);

/* Global variable to hold the durations array for comparison */
static int *durations;

/* Implementação da biblioteca */
void heapify(Maquina v[], int m, int i) {
    int smallest = i;
    int left = 2 * i + 1;   
    int right = 2 * i + 2;

    if (left < m && v[left].carga < v[smallest].carga)
        smallest = left;
    if (right < m && v[right].carga < v[smallest].carga)
        smallest = right;

    if (smallest != i) {
        Maquina temp = v[i];
        v[i] = v[smallest];
        v[smallest] = temp;
        heapify(v, m, smallest);
    }
}

void buildHeap(Maquina v[], int m) {
    for (int i = (m / 2) - 1; i >= 0; i--)
        heapify(v, m, i);
}

// Returns the id of the machine with the minimum carga
int min(Maquina v[]) {
    return v[0].id;
}

// Updates the heap after increasing the carga of the minimal machine
void add_to_minkey(int m, Maquina v[], int delta) {
    v[0].carga += delta;
    heapify(v, m, 0);
}

int comparacao(const void *a, const void *b) {
    int idxA = *(const int *)a;
    int idxB = *(const int *)b;
    return durations[idxA] - durations[idxB];
}

void sortInd(int n, int d[], int id[]) {
/* ordena o vetor id[0..n-1] de modo que d[id[0]] <= d[id[1]] <= ... <= d[id[n-1]] */

    for (int i = 0; i < n; i++)
        id[i] = i;

    // Set the global durations array for comparison
    durations = d;

    // Sort id array based on durations
    qsort(id, n, sizeof(int), comparacao);
}

/*devolve um vetor com o escalonamento em m máquina obtido pelo algoritmo de Graham 
das tarefas 0,...,n-1 com durações dadas no vetor d[0..n-1].*/
int *Graham(int m, int n, int d[]) {
    // Allocate memory for the schedule array
    int *schedule = malloc(n * sizeof(int));

    // Initialize the heap with m machines
    Maquina *heap = malloc(m * sizeof(Maquina));

    for (int i = 0; i < m; i++) {
        heap[i].id = i;
        heap[i].carga = 0;
    }

    // Build the initial heap
    buildHeap(heap, m);

    // Initialize the id array
    int *id = malloc(n * sizeof(int));
    if (id == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    sortInd(n, d, id);

    // For each task
    for (int i = 0; i < n; i++) {
        int task_idx = id[i];  // Use id[i] if tasks are sorted
        int task_duration = d[task_idx];

        // Get the machine with the minimum carga
        int machine_id = min(heap);

        // Assign the task to this machine
        schedule[task_idx] = machine_id;

        // Update the carga of this machine
        add_to_minkey(m, heap, task_duration);
    }

    // Free heap memory
    free(heap);
    free(id);

    return schedule;
}

/*devolve um vetor com o escalonamento em m máquina obtido pelo algoritmo de Graham 
das tarefas 0,...,n-1 com durações dadas no vetor d[0..n-1], 
escalonando as tarefas por ordem decrescente de duração.*/

int compare_desc(const void *a, const void *b) {
    int idxA = *(const int *)a;
    int idxB = *(const int *)b;
    return durations[idxB] - durations[idxA];
}

void sort_tasks_desc(int n, int d[], int idx[]) {
    for (int i = 0; i < n; i++)
        idx[i] = i;

    // Set the global durations array for comparison
    durations = d;

    qsort(idx, n, sizeof(int), compare_desc);
}

int *sortedGraham(int m, int n, int d[]) {
    int *schedule = malloc(n * sizeof(int));
    if (schedule == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    // Initialize the heap with m machines
    Maquina *heap = malloc(m * sizeof(Maquina));
    if (heap == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < m; i++) {
        heap[i].id = i;
        heap[i].carga = 0;
    }

    // Sort tasks in decreasing order of duration
    int *idx = malloc(n * sizeof(int));
    if (idx == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    sort_tasks_desc(n, d, idx);

    // For each task in sorted order
    for (int i = 0; i < n; i++) {
        int task_idx = idx[i];
        int task_duration = d[task_idx];

        // Get the machine with the minimum carga
        int machine_id = min(heap);

        // Assign the task to this machine
        schedule[task_idx] = machine_id;

        // Update the carga of this machine
        add_to_minkey(m, heap, task_duration);
    }

    free(heap);
    free(idx);

    return schedule;
}
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

  -  adaptada de:
       

 \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/

#include <stdio.h>
#include "heap.c"
#include <stdlib.h>

int main(int argc, char *argv[]) {
    /* argc = numero de argumentos na linha de comando */
    /* argv = vetor de apontadores para strings contendo esses argumentos */
 
    FILE *entrada; /* declaracao da variavel para o arquivo de entrada */
    int m, n;      /* Number of machines and tasks */
    int *d;        /* Array to store task durations */
    int *schedule; /* Declaração da variável schedule */
    int i;         /* Variável de loop */
      
    if (argc == 1) {
        printf("Uso: %s <arquivo de entrada>\n", argv[0]);
        return -1;
    }          

    if ((entrada = fopen(argv[1],"r")) == NULL) {
        printf("%s: arquivo de entrada %s nao pode ser aberto.\n", argv[0], argv[1]);
        return -1;
    }

    /* Read the values of m and n from the first line */
    fscanf(entrada, "%d %d", &m, &n);

    /* Allocate memory for the durations array */
    d = (int *)malloc(n * sizeof(int));

    /* Read the task durations from the second line */
    for (i = 0; i < n; i++)
        fscanf(entrada, "%d", &d[i]);
        
    fclose(entrada);

    /* Call the Graham function */
    schedule = Graham(m, n, d);

    /* Output the schedule */
    printf("Task assignments to machines:\n");
    for (i = 0; i < n; i++) {
        printf("Task %d assigned to Machine %d\n", i, schedule[i]);
    }

    /* Free allocated memory */
    free(schedule);

    // Schedule using sortedGraham
    schedule = sortedGraham(m, n, d);

    // Print tasks in decreasing order of duration
    int *idx = (int *)malloc(n * sizeof(int));
    if (idx == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria.\n");
        free(d);
        return -1;
    }
    sort_tasks_desc(n, d, idx);

    printf("Tarefas em ordem decrescente de duracao:\n");
    for (int i = 0; i < n; i++) {
        printf("Tarefa %d: Duracao %d\n", idx[i], d[idx[i]]);
    }

    printf("Escalonamento produzido pela rotina sortedGraham:\n");
    for (int i = 0; i < n; i++) {
        printf("Tarefa %d atribuida a Maquina %d\n", i, schedule[i]);
    }

    free(d);
    free(schedule);
    free(idx);

    return 0;
}
/*Primeiro faça uma versão preliminar do algoritmo de Graham que não usa heap. 
Determine qual é a máquina menos carregada em tempo linear no número de máquinas. 
Apenas essa parte já deve valer 3.0 pontos. Tente estar com isso pronto no fim desta semana.

Depois que fizer isso, você pode escolher: faça primeiro a ordenação indireta e o sortedGraham, 
ou então faça a implementação do heap.c e o modifique o Graham para usá-la. 
Cada uma destas partes deve valer 3.5 pontos.*/
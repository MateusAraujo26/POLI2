/* ************************************************************ */
/*              Interface para o EP4: heap.h                    */
/* ************************************************************ */
/*        Nao faca nenhuma alteracao neste arquivo.             */
/* ************************************************************ */

struct maquina {
  int id; 
  int carga; 
};

typedef struct maquina Maquina;

int min(Maquina v[]);
/* devolve o id da mÃ¡quina com a carga mÃ­nima do heap v */

void add_to_minkey(int m, Maquina v[], int delta);
/* reorganiza o heap v[0..m-1] depois de aumentar a carga da mÃ¡quina de carga mÃ­nima de delta */
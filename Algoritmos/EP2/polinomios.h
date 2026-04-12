/* ****************************************************************** */
/*              Interface para o EP2: polinomios.h                    */
/* ****************************************************************** */
/*              Nao faca nenhuma alteracao neste arquivo.             */
/* ****************************************************************** */

typedef void *polinomio;

polinomio cria();

polinomio leia();

polinomio copia(polinomio);

void      impr(char, polinomio);

polinomio soma(polinomio, polinomio);

polinomio subt(polinomio, polinomio);

polinomio nega(polinomio);

polinomio mult(polinomio, polinomio);

polinomio quoc(polinomio, polinomio);

polinomio rest(polinomio, polinomio);

void libera(polinomio);
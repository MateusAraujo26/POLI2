#include <stdio.h>

int fatorial(int n);

/**
* Ponto de entrada do programa (entry point).
*/
  int main(int argc, char** argv) {
  int n;
  scanf("%d", &n);

  printf("%d\n", fatorial(n));

  return 0;
}

/**
* Calcula o fatorial do número recebido e retorna o resultado.
*/
int fatorial(int n) {
  int fat = 1;
  int i;
  for(i = 2; i < (n+1); i ++)
    fat = fat*i;
  return fat;
}
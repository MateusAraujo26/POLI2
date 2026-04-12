#include <stdio.h>
#include "polinomios.h"

void printMenu() {
  printf("\n");
  printf("0: cria e mostra um polinomio nulo\n");
  printf("l: le e mostra um polinomio\n");
  printf("c: le, copia e mostra a copia de um polinomio\n");
  printf("+: le dois polinomios e mostra sua soma\n");
  printf("-: le dois polinomios e mostra sua diferenca\n");
  printf("*: le dois polinomios e mostra seu produto\n");
  printf("/: le dois polinomios e mostra o quociente da sua divisao\n");
  printf("%%: le dois polinomios e mostra o resto da sua divisao\n");
  printf("n: le um polinomio e mostra o seu negativo\n");
  printf("q: sair do programa\n\n");
  printf("Digite a opcao desejada: ");
}

int main() {
    char opcao;
    polinomio p, q, r;

    printf("******************************************\n");
    printf("Calculadora de polinomios - fase de testes\n");
    printf("******************************************\n");

    printMenu();

    scanf(" %c", &opcao);

    while (opcao != 'q') {
        switch (opcao) {
        case '0':
            p = cria();
            impr('p', p);
            libera(p);
            break;
        case 'l': case 'c': case 'n': /* operacoes unarias */
            printf("\nDigite o coeficiente e o expoente de cada termo de um polinomio, seguido de um 0.\n");
            p = leia();
            if (opcao == 'l') /* leitura */
                impr('p', p);
            else {
                if (opcao == 'c') /* copia */
                    r = copia(p);
                else /* nega */
                    r = nega(p);
                impr('p', p);
                impr('r', r);
                libera(r);
            }
            libera(p);
            break;
        case '+': case '-': case '*': case '/': case '%': /* operacoes binarias */
            printf("\nDigite o coeficiente e o expoente de cada termo do primeiro polinomio, seguido de um 0.\n");
            p = leia();
            printf("Digite o coeficiente e o expoente de cada termo do segundo polinomio, seguido de um 0.\n");
            q = leia();
            switch (opcao) {
            case '+': /* soma */
                r = soma(p,q);
                break;
            case '-': /* subtracao */
                r = subt(p,q);
                break;
            case '*': /* produto */
                r = mult(p,q);
                break;
            case '/': /* divisao */
                r = quoc(p,q);
                break;
            case '%': /* resto da divisao */
                r = rest(p,q);
            }
            impr('p', p);
            impr('q', q);
            impr('r', r);
            libera(p);
            libera(q);
            libera(r);
            break;
        case 'q':
            break;
        default:
            printf("Opcao invalida!\n");
        }
        printMenu();
        scanf(" %c", &opcao);
    }
    printf("Tchau!\n");
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.c"
#include "polinomios.c"

int main() {
    char infixa[100];
    printf("Enter an infix expression: ");
    fgets(infixa, sizeof(infixa), stdin);

    // Remove newline character from the input
    size_t len = strlen(infixa);
    if (len > 0 && infixa[len - 1] == '\n') {
        infixa[len - 1] = '\0';
    }

    char *posfixa = infixaParaPosfixa(infixa);
    if (posfixa != NULL) {
        printf("Postfix expression: %s\n", posfixa);
        free(posfixa); // Free the allocated memory for the postfix expression
    } else {
        printf("Error converting infix to postfix.\n");
    }

    return 0;
}
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void escrever(char linha[]);
void removerMemoria();

int main(){
    int enderecoMemoria = 0;
    int qtde = 0;
    char valor[5];
    char posicao[9];
    removerMemoria();
    printf("Quantidade de valores:\n");
    scanf("%d", &qtde);

    for(int i = 0; i < qtde; i++){
        //printf("Digite o valor %d: ",i+1);
        scanf("%s", valor);
        sprintf(posicao, "%04X %s", enderecoMemoria, valor);
        escrever(posicao);
        enderecoMemoria+=2;
        //printf("linha: %s\n", posicao);
    }

    printf("Valores salvos na memoria.\n");
    return 0;
}

void escrever(char linha[]){
    FILE* memoria;
    memoria = fopen("mem.out","a"); 

    if (memoria == NULL){
        printf("O arquivo nao esta aberto.");
    }else{
        fputs(linha, memoria);
        fputs("\n", memoria);
        fclose(memoria);
    }
}

void removerMemoria(){
    FILE* memoria;
    memoria = fopen("mem.out","r"); 
    if (memoria){
        fclose(memoria);
        remove("mem.out");
    }
}
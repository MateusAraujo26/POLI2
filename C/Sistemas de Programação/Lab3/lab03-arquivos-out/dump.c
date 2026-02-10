#include <stdio.h>
#include <stdlib.h>


int main(){
    char instrucoes[100];
    FILE* memoria;
    memoria = fopen("mem.out","r");

    if(memoria == NULL){
        printf("O arquivo nao esta aberto");
    }else{
        while(fgets(instrucoes,100,memoria) != NULL){
            printf("%s", instrucoes);
        }
        fclose(memoria);
    }

    return 0;
}
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(){
    char instrucoes[100];
    char endereco[5];
    int counter = 0;

//    printf("Digite o primeiro endereco: \n");
    unsigned int endereco1, endereco2, valor1, valor2;
    scanf("%X", &endereco1);
//    printf("Digite o segundo endereco: \n");
    scanf("%X", &endereco2);

    FILE* memoria;
    memoria = fopen("mem.out","r");

    if(memoria == NULL){
        printf("O arquivo nao esta aberto");
    }else{
        while(fgets(instrucoes,sizeof(instrucoes),memoria) != NULL){

            strncpy(endereco, instrucoes + counter, 4);
            endereco[5] = '\0';
            char strEndereco1[5];
            char strEndereco2[5];
            char strValor1[5];
            char strValor2[5];
            snprintf(strEndereco1, sizeof strEndereco1, "%04X", endereco1);
            snprintf(strEndereco2, sizeof strEndereco2, "%04X", endereco2);

            if(strcmp(endereco,strEndereco1) == 0){
                strncpy(strValor1, instrucoes + 5, 4);
                //printf("val: %s\n", strValor1);
                //printf("end: %s\n", endereco);
                valor1 = strtol(strValor1, NULL, 16);
            }
            if(strcmp(endereco,strEndereco2) == 0){
                strncpy(strValor2, instrucoes + 5, 4);
                //printf("val: %s\n", strValor2);
                //printf("end: %s\n", endereco);
                valor2 = strtol(strValor2, NULL, 16);
            }
        }
        fclose(memoria);
    }
//    printf("val1: %X, val2: %X ", valor1, valor2);
//    printf("INT%d\n",(valor1 + valor2));
    printf("%04X\n",(valor1 + valor2));

    return 0;
}
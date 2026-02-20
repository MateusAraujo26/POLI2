/*
No laboratório de hoje, vamos construir novos programas para realizar algumas operações básicas de um computador, 
além de aproveitar os programas feitos nos laboratórios anteriores. 
Vocês também vão escrever um programa simulador para realizar essas operações por meio de instruções padronizadas.
*/

/*
Exercício 4 (2,0 pontos): Crie o programa simulador (simulador.c).
 O simulador recebe uma instrução de 4 dígitos, em que o dígito mais significativo é o opcode 
 e os demais 3 dígitos são um endereço da memória. 
 Por exemplo, na instrução 3010, o dígito “3” é a operação a ser realizada e “010” é o endereço da memória. 
 Portanto, a partir daqui trabalharemos com endereços de memória de 3 dígitos, 
 mas não é necessário fazer nenhuma modificação no formato do arquivo mem.out.

No simulador, devem ser implementadas 4 operações, que chamam os seguintes programas: 

4: sum
8: acc
9: mem
E: dump
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    unsigned short opcode, endereco;
    sscanf(argv[1], "%1hX%3hX", &opcode, &endereco);

    char comando[50];

    switch (opcode)
    {
    case 0x4:
        sprintf(comando, "./sum %04X", endereco);
        system(comando);
        break;
    case 0x8:
        sprintf(comando, "./acc %04X", endereco);
        system(comando);
        break;
    case 0x9:
        sprintf(comando, "./mem %04X", endereco);
        system(comando);
        break;
    case 0xE:
        system("./dump");
        break;
    }
    return 0;    
}
#define COLS 2

#include <stdio.h>
#include <string.h>

int gMACtria(char c);
int Numetrica(char texto[], int seqnum[]);
int Busca(int tabela[][COLS], int n, int numetria);
int Tabula(int seqnum[], int n, int tabela[][COLS], int m);

int contador_palavras = 0; // variavel global para contar palavras

int gMACtria(char c)
{
    int valores[26] = 
    {1, 2, 3, 4, 5, 6, 7, 8, 9,
     10, 20, 30, 40, 50, 60, 70, 80, 90,
     100, 200, 300, 400, 500, 600, 700, 800};

    if(c >= 'a' && c<= 'z'){
        return valores[c - 'a'];
    }
    else{
        return 0;
    }
}

int Numetrica(char texto[], int seqnum[]) {
    int i, index = 0;
    int tamanho = 0;
    int gmactria, numetria_palavra = 0;
    contador_palavras = 0; //zera contador_palavras
    for (i = 0; texto[i] != '\0'; i++){ //se a string nao tiver terminado ('\0')
        gmactria = gMACtria(texto[i]);
        if (gmactria != 0){
            numetria_palavra += gmactria; //se for um caractere, aumenta a numetria
        }
        else {
            if (numetria_palavra != 0){ //se for carcatere invalido e a numetria nao for 0
                contador_palavras++; //vai aumentar contador_palavras, guarda valor em seqnum e zerar a numetria
                seqnum[index] = numetria_palavra;
                numetria_palavra = 0; //assim se for um caractere em dobro, nao contara
                index++;
            }
        }
    }
    if (numetria_palavra != 0){ //se loop terminar e a numetria nao for 0 (termina em letra)
            contador_palavras++; //aumenta contador e guarda palavra
            seqnum[index] = numetria_palavra;
    }
    return i; //retorna tamamnho do vetor
}

int Busca(int tabela[][COLS], int n, int numetria) {
    int i, indice = 0;
    for (i = 0; i < n; i++) {
        if (tabela[i][0] == 0) {
            indice--;
        }
        if (tabela[i][0] == numetria) {
            return (indice + i);
        }
    }
    return -1;
}

int tabula(int seqnum[], int n, int tabela[][COLS], int m) {
    int i, j, count = 0;
    for (i = 0; i < n; i++) {
        tabela[i][0] = seqnum[i];
        tabela[i][1] = 1;
    }
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            if (seqnum[i] == seqnum[j]) {
                tabela[i][1]++;
                tabela[j][0] = 0;
            }
        }
    }
    for (i = 0; i < n; i++) {
        if (tabela[i][0] != 0) {
            count++;
        }
    }
    return count;
}

int zerar_vetor(int seqnum[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        seqnum[i] = 0;
    }
    return 0;
}

int main() {
    int seqnum[1000] = {0};
    int f = 0;
    int count = 0;
    while (f != 7) {
        printf("Selecione a funcionalidade f do programa (1-7): ");
        scanf("%d", &f);
        switch (f) {
            case 1: {
                char c;
                printf("Digite um caractere: ");
                scanf(" %c", &c);
                printf("A gMACtria de '%c' eh %d\n", c, gMACtria(c));
                break;
            }
            case 2: {
                int i;
                char palavra[50] = {"\0"};
                int numetria = 0;
                printf("Digite uma palavra: ");
                scanf(" %s", palavra);
                for (i = 0; palavra[i] != '\0'; i++) {
                    numetria += gMACtria(palavra[i]);
                }
                printf("A numetria de '%s' eh %d\n", palavra, numetria);
                break;
            }
            case 3: 
                char texto[1000] = {"\0"};
                int i, length;
                zerar_vetor(seqnum, 1000);
                printf("Digite um texto com ateh 1000 letras:\n");
                scanf(" %[^\n]", texto);
                length = Numetrica(texto, seqnum);
                printf("Sequencia numetrica com codigos das %d palavras:\n", contador_palavras);
                for (i = 0; i < length; i++) {
                    if (seqnum[i] == 0) {
                        break;
                    }
                    else{
                        printf("%d ", seqnum[i]);
                    }
                }
                printf("\n");
                break;
            
            case 4: 
                zerar_vetor(seqnum, 1000);
                int N;
                printf("Digite N e uma sequencia com N inteiros/numetrias:\n");
                scanf("%d", &N);
                for (i = 0; i < N; i++){
                    scanf("%d", &seqnum[i]);
                }
                printf("Sequencia com %d numeros armazenada\n", N);
                length = N;
                break;
            
            case 5:
                int matriz[1000][2] = {{0}};
                count = tabula(seqnum, length, matriz, 1000);
                printf("A sequencia tabulada possui %d pares de numetrias e contadores:\n", count);
                for (i = 0; i < length; i++){
                    if (matriz[i][0] != 0){
                        printf("%4d %4d\n", matriz[i][0], matriz[i][1]);
                    }
                }
                break;
            
            case 6: 
                int numetria_input;
                printf("Digite inteiro a ser procurado na primeira coluna da tabela: ");
                scanf("%d", &numetria_input);
                int indice = Busca(matriz, length, numetria_input);
                printf("A posicao encontrada eh %d\n", indice);
                break;
                        
        }
    }
    printf("Tchau.");
}
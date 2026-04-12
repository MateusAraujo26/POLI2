/***************************************************************/
/**                                                           **/
/**   Nome:       Gabriel Agra De Castro Motta                **/
/**   Professor:  Flavio Soares Correa da Silva               **/
/**   Turma:      01                                          **/
/**   NumeroUSP:  15452743                                    **/
/**   Exercicio-Programa 01                                   **/
/**                                                           **/
/***************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main(){
    int f, n, i;
    /*f escolhe a funcionalidade do programa, n eh o numero input*/
    printf("\nSelecione a funcionalidade f do programa (0, 1, 2 ou 3): ");
    scanf("%d", &f);

    if(f==0){
        printf("Entre com um inteiro a testar se eh primo: ");
        scanf("%d", &n);
        for (i = 2; i*i <= n; i++){
            if(n % i == 0){
                printf(" O numero %d nao eh primo", n);
                return 0;
            }
        }
        printf("O numero %d eh primo", n);
        return 0;
    }
    if(f==1){
        int k, p, j;
        /*k eh o numero de primos, p eh o primo*/
        int l = 0;
        /*l eh o contador de primos achados, precisa se igualar a k*/
        int primo = 1;
        /*primo eh um boolean que diz se o numero eh primo ou nao, serve pra quebrar o loop */
        printf("Entre com n e k: ");
        scanf("%d", &n);
        scanf("%d", &k);

        if(k > 0){
            for (i= n + 1; i <= 2147483647; i++){
                /*loop pra testar numeros entre n e 2147483647, incrementando 1*/
                for (j = 2; j*j <= i; j++){
                        /* loop pra ver se i eh primo, divide por todos os numeros entre 2 e raiz de i */
                        if(i % j == 0){
                            /* se a divisao for exata, nao eh primo, retorna o boolean primo pra falso e sai do lop */
                            primo = 0;
                            break;
                        }
                        else{
                            primo = 1;
                        }
                    }
                    if(primo){
                        /* se for primo, atualiza o valor de p pro primo i. se nosso contador de primos l for igual k, entao teremos nosso primo */
                        p = i;
                        l++;
                        if(l==k){
                            printf("O %d-esimo primo maior que %d eh %d", k, n, p);
                            return 0;
                        }
                    }
                    if (i == 2147483647 && l < k)
                    {
                        printf("Nao existe(m) %d primo(s) entre %d e 2147483647", k, n);
                        return 0;
                    }
                }
            }

            if(k < 0){
                k= abs(k);
                /* modulo de k */
                for (i= n - 1; i >= 2; i--){
                    for (j = 2; j * j <= i; j++){
                        if(i % j == 0){
                            primo = 0;
                            break;
                        }
                        else{
                            primo = 1;
                        }
                    }
                    if(primo){
                        p = i;
                        l++;
                        if(l==k){
                            printf("O %d-esimo primo menor que %d eh %d", k, n, p);
                            return 0;
                        }
                    }
                }
                    printf("Nao existe(m) %d primo(s) entre 0 e %d", k, n);
                    return 0;
            }
        }
        if (f==2){
            int l = 0;
            int length = 0;
            int primo = 1;
            int k, j, p, p2, res;
            printf("Entre com n e k: ");
            scanf("%d", &n);
            scanf("%d", &k);

            if(k > 0){
                for (i= n + 1; i <= 2147483647; i++){
                    for (j = 2; j*j <= i; j++){
                        if(i % j == 0){
                            primo = 0;
                            break;
                        }
                        else{
                            primo = 1;
                        }
                    }
                    if(primo){
                        p = i;
                        l++;
                        if(l==k){
                        /*usa divisao simples pra saber quantos digitos tem o numero: quando o divisor for maior que dig, dig virara 0
                        incrementa 1 a length a cada divisao*/
                            int dig = p;
                            while (dig != 0) {
                                dig /= 10;
                                length++;
                            }
                            printf("O %d-esimo primo maior que %d eh %d e possui %d digito(s)", k, n, p, length);
                            break;
                        }
                    }
                    if (i == 2147483647 && l < k)
                    {
                        printf("Nao existe(m) %d primo(s) entre %d e 2147483647", k, n);
                        break;
                    }
                }
            }

            if(k < 0){
                k= abs(k);
                for (i= n - 1; i >= 2; i--){
                    for (j = 2; j * j <= i; j++){
                        if(i % j == 0){
                            primo = 0;
                            break;
                        }
                        else{
                            primo = 1;
                        }
                    }
                    if(primo){
                        p = i;
                        l++;
                        if(l==k){
                            int dig = p;
                            while (dig != 0) {
                                dig /= 10;
                                length++;
                            }
                            printf("O %d-esimo primo menor que %d eh %d e possui %d digito(s)", k, n, p, length);
                            break;
                        }
                    }
                    if (i == 2 && l < k){
                        printf("Nao existe(m) %d primo(s) entre 0 e %d", k, n);
                        break;}
                }      
            }
            l = 0;
            length = 0;
            i = 0;
            j = 0;
            printf("\nEntre com n e k: ");
            scanf("%d", &n);
            scanf("%d", &k);
            if(k > 0){
                for (i= n + 1; i <= 2147483647; i++){
                    for (j = 2; j*j <= i; j++){
                        if(i % j == 0){
                            primo = 0;
                            break;
                        }
                        else{
                            primo = 1;
                        }
                    }
                    if(primo){
                        p2 = i;
                        l++;
                        if(l==k){
                            int dig = p2;
                            while (dig != 0) {
                                dig /= 10;
                                length++;
                            }
                            printf("O %d-esimo primo maior que %d eh %d e possui %d digito(s)", k, n, p2, length);
                            break;
                        }
                    }
                    if (i == 2147483647 && l < k)
                    {
                        printf("Nao existe(m) %d primo(s) entre %d e 2147483647", k, n);
                        break;
                    }
                }
            }
            if(k < 0){
                k= abs(k);
                for (i= n - 1; i >= 2; i--){
                    for (j = 2; j * j <= i; j++){
                        if(i % j == 0){
                            primo = 0;
                            break;
                        }
                        else{
                            primo = 1;
                        }
                    }
                    if(primo){
                        p2 = i;
                        l++;
                        if(l==k){
                            int dig = p2;
                            while (dig != 0) {
                                dig /= 10;
                                length++;
                            }
                            printf("O %d-esimo primo menor que %d eh %d e possui %d digito(s)", k, n, p2, length);
                            break;
                        }
                    }
                    if (i == 2 && l < k){
                        printf("Nao existe(m) %d primo(s) entre 0 e %d", k, n);
                        break;}
                }      
            }
            
            res = p*p2;
            /*resultado do produto dos dois*/
            if (res < 0)
            {
                printf("\nO produto dos dois primos eh maior que 2147483647");
            }
            else{
                int dig = res;
                length = 0;
                /* reinicia length para 0 */
                while (dig != 0) {
                    dig /= 10;
                    length++;
                            }
                printf("\nO produto dos dois primos eh %d e possui %d digito(s)", res, length);
            }
        }
        if (f == 3){
            int primo, p2, p;
            printf("Entre com um inteiro a testar se se eh produto de dois primos: ");
            scanf("%d", &n);

            /*checa se n eh primo: se for, nao vai ter fatoress e fecha programa*/
            primo = 1;
            for (i = 2; i * i <= n; i++) {
                if (n % i == 0) {
                    primo = 0;
                    break;
                }
            }

            if (primo) {
                printf("O numero %d nao eh produto de dois primos", n);
                return 0;
            } else {
                /*vai tentar achar dois fatores que sejam primos: primeiro tenta achar um, procurando um divisor entre 2 e raiz de n*/
                for (p = 2; p * p <= n; p++) {
                    if (n % p == 0) {
                        primo = 1;
                        for (i = 2; i * i <= p; i++) {
                            /*depois vai testar se esse divisor eh primo*/
                            if (p % i == 0) {
                                primo = 0;
                                break;
                            }
                        }
                        if (primo) {
                            break;
                        /*se achar um divisor primo, pode sair do loop*/
                        }
                    }
                }
                /*o outro divisor eh p2, n/p*/
                p2 = n / p;

                /*ai so checar se p2 tambem eh primo*/
                primo = 1;
                for (i = 2; i * i <= p2; i++) {
                    if (p2 % i == 0) {
                        primo = 0;
                        break;
                    }
                }
                /*se p2 for primo*/
                if (primo) {
                    printf("O numero %d eh produto dos dois primos %d e %d", n, p, p2);
                /*se nao for*/
                } else {
                    printf("O numero %d nao eh produto de dois primos", n);
                }
            }
        }
    }
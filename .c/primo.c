#include <stdio.h>

int eh_primo(long long int n){
    if(n == 2 || n == 3)
        return 1;
    if((n - 1) % 6 != 0 && (n + 1) % 6 != 0)
        return 0;
    long long int i;
    for (i = 5; i * i < n; i += 6){
        if(n % i == 0 || n % (i + 2) == 0)
            return 0;
    }
    return 1;
}

int main(){
    long long int n = 1;
    while(n != 0){
        printf("Insira:");
        scanf("%lld", &n);
        if (eh_primo(n))
            printf("%lld eh primo\n", n);
        else
            printf("%lld nao eh primo\n", n);
    }
}
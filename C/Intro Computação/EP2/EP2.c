/***************************************************************/
/**                                                           **/
/**   Nome:       Gabriel Agra De Castro Motta                **/
/**   Professor:  Flavio Soares Correa da Silva               **/
/**   Turma:      01                                          **/
/**   NumeroUSP:  15452743                                    **/
/**   Exercicio-Programa 02                                   **/
/**                                                           **/
/***************************************************************/

#include <stdio.h>

double Log2(double z, double eps){
    /*constante de 2 / ln 2*/
    double cte = 2 / 0.693147180559945;
    /*inicializaremos o resultado inicial como 0*/
    double resultado = 0;
    double resultado_log = 0;
    /*inicializamos i para numero do expoente e divisor da fracao, comeca em 1*/
    int i = 1;
    /*declaramos uma variavel base para salvar o valor da divisao.*/
    double base = (z - 1) / (z + 1);
    /*declaramos uma variavel pow para guardar o valor das potencias*/
    double pow;
    /*declaramos fracao para guardar o valor da parte fracionaria.*/
    double fracao = 1;

    /*como i comeca em 1, podemos tirar as seguintes conclusoes*/
    pow = base;
    resultado += base;
    
    i+=2;
    while (fracao >= eps || -fracao >= eps){
        /*pegaremos o valor de pow guardado na ultima iteracao e apenas
        multiplicaremos pela base duas vezes, ganhando eficiencia*/
        pow *= base;
        pow *= base;
        fracao = pow / i;
        /*atualiza resultado com a nova fracao calculada*/
        resultado += fracao;
        i+=2;
    }
    resultado += fracao;
    resultado_log = resultado * cte;
    return resultado_log;
}

double InformacaoMutua(int N11, int N10, int N01, int N00, double eps){
    int N = N00 + N01 + N10 + N11;

    double prob_N00 = (double)N00 / N;
    double prob_N01 = (double)N01 / N;
    double prob_N10 = (double)N10 / N;
    double prob_N11 = (double)N11 / N;

    double prob_x1 =  (double)(N10 + N11)/N;
    double prob_x0 = (double)(N00 + N01) / N;
    double prob_y1 = (double)(N01 + N11) / N;
    double prob_y0 = (double)(N00 + N10) / N;

    double IM_11 = prob_N11 * Log2(prob_N11 / (prob_x1 * prob_y1), eps);
    double IM_10 = prob_N10 * Log2(prob_N10 / (prob_x1 * prob_y0), eps);
    double IM_01 = prob_N01 * Log2(prob_N01 / (prob_x0 * prob_y1), eps);
    double IM_00 = prob_N00 * Log2(prob_N00 / (prob_x0 * prob_y0), eps);
    double IM = IM_11 + IM_10 + IM_01 + IM_00;
    return IM;
}

int main(){
    int f;
    double eps;
    printf("Selecione a funcionalidade f do programa (1 ou 2): ");
    scanf ("%d", &f);
    if(f == 1){
        double z;
        double log_2_z;
        printf("Entre com x e eps: ");
        scanf("%lf %lf", &z, &eps);
        log_2_z= Log2(z, eps);
        printf("O logaritmo de %g com precisao %g eh %g", z, eps, log_2_z);

    }
    else{
        int N11, N10, N01, N00;
        double info_m;
        printf("Entre com contadores N11, N10, N01, N00 e com eps: ");
        scanf("%d %d %d %d %lf", &N11, &N10, &N01, &N00, &eps);
        info_m = InformacaoMutua(N11, N10, N01, N00, eps);
        printf("A informacao mutua entre as duas variaveis aleatorias eh %g", info_m);
    }
}
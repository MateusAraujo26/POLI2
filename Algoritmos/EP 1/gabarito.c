#include <stdio.h>
#include <math.h>

FILE *arg;

void koch(int n, float size) {
    if (n == 0) {
        fprintf(arg, "%f 0 rlineto\n", size);
    } else {
        float x = size / 3;
        koch(n-1, x);
        fprintf(arg, "60 rotate\n");
        koch(n-1, x);
        fprintf(arg, "-120 rotate\n");
        koch(n-1, x);
        fprintf(arg, "60 rotate\n");
        koch(n-1, x);
    }
    return;
}

void floco_de_koch(int n, float size){
    fprintf(arg, "60 rotate\n");
    koch(n,size);
    fprintf(arg, "-120 rotate\n");
    koch(n,size);
    fprintf(arg, "-120 rotate\n");
    koch(n,size);
    return;
}

void anti_floco_de_koch(int n, float size){
    fprintf(arg, "-60 rotate\n");
    koch(n,size);
    fprintf(arg, "120 rotate\n");
    koch(n,size);
    fprintf(arg, "120 rotate\n");
    koch(n,size);
    return;
}

void canopy(int n, float size){
    if (n == 0) {
        fprintf(arg, "0 %f rlineto\n", size);
        return;
    } else {
        fprintf(arg, "0 %f rlineto\n", size);
        fprintf(arg, "20 rotate\n");
        canopy(n-1,3*size/4);
        fprintf(arg, "0 %f rlineto\n", -3*size/4);
        fprintf(arg, "-40 rotate\n");
        canopy(n-1,3*size/4);
        fprintf(arg, "0 %f rlineto\n", -3*size/4);
        fprintf(arg, "20 rotate\n");
    }
}

void dragao(int n, float size, int aux) {
    if (n == 0) {
        fprintf(arg, "%f 0 rlineto\n", size);
        return;
    } if(n==1) {
        fprintf(arg, "%f 0 rlineto\n", size);
        if(aux == 1){
            fprintf(arg, "90 rotate\n");
        }  else {
            fprintf(arg, "-90 rotate\n");
        }
        fprintf(arg, "%f 0 rlineto\n", size);
    } else {
        dragao(n-1, size/sqrt(2), 1);
        if(aux == 1){
            fprintf(arg, "90 rotate\n");
        }  else {
            fprintf(arg, "-90 rotate\n");
        }
        dragao(n-1, size/sqrt(2), -1);
    }
}

void meu_fractal(int n, float size){
    if (n < 0) {
        return;
    } else {
        size/=1.618;
        meu_fractal(n-1, size);
        fprintf(arg, "%f 0 rlineto\n", size);
        fprintf(arg, "120 rotate\n");
        fprintf(arg, "%f 0 rlineto\n", size);
        fprintf(arg, "120 rotate\n");
        fprintf(arg, "%f 0 rlineto\n", size);
        fprintf(arg, "120 rotate\n");
        fprintf(arg, "%f 0 rlineto\n", size);
        fprintf(arg, "60 rotate\n");
    }
}

int main() {
    int n, f;
    float size = 300;
    char c[100];
    scanf("%d %d %s", &f, &n, c);
    arg = fopen(c, "w");
    /* anti-floco de neve de Koch */
    if(f == 0) {
        fprintf(arg, "200 500 moveto\n");
        anti_floco_de_koch(n,size);
    }
    /* árvore de canopy */
    else if(f == 1){
        size = 100;
        fprintf(arg, "300 300 moveto\n");
        canopy(n,size);
    }
    /* curva do dragão */
    else if(f == 2){
        size = 100;
        fprintf(arg, "200 500 moveto\n");
        for(int i=0;i<n;i++){
            fprintf(arg, "-45 rotate\n");
        }
        dragao(n,size,1);
    }
    else if(f == 3){
        size=500;
        fprintf(arg, "400 200 moveto\n");
        meu_fractal(n,size);
    }
    fprintf(arg, "stroke\n");

    fclose(arg);
    return 1;
}

#include <stdio.h>

void flecha(int x){
    for(int i=0; i<x/2; i++){
        for(int j=(x/2)-1; j>=i; j--){
            printf(" ");
        }
        for(int j=0; j<=(i*2); j++){
            printf("*");
        }
        for(int j=0; j<(x/2)-i; j++){
            printf(" ");
        }
        for(int j=0; j<x/2; j++){
            printf(" ");
        }
        for(int j=0; j<1; j++){
            printf("*");
        }
        printf("\n");
    }

    for(int i=0; i<x*2; i++){
        printf("*");
    }
    printf("\n");

    for(int i=0; i<x/2; i++){
        for(int j=0; j<x/2; j++){
            printf(" ");
        }
        for(int j=0; j<1; j++){
            printf("*");
        }
        for(int j=0; j<(x/2)+i+1; j++){
            printf(" ");
        }
        for(int j=x; j>(i*2)+2; j--){
            printf("*");
        }
        printf("\n");
    }
}

int main(){

    int n;

    printf("Ingresa el tamanio de la base\n");
    scanf("%d",& n);

    if(n % 2 == 0){
        n-=1;
        printf("Se resto uno para poder realizar la flecha\n");
    }

    flecha(n);

    return 0;
}
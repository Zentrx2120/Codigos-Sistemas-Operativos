#include <stdio.h>
#include <math.h>

void arrow(int);

int main(){

    int n;

    printf("Escriba las bases de las flechas\n");
    scanf("%d",&n);

    if(n%2 !=0 ){
        n+=1;
    }

    arrow(n);

    return 0;
}

void arrow(int x){
    //printf("%d", x);
    for(int k = 0; k < x / 2; k++){
        for(int j = 0; j < x / 2 + 1; j++){
            printf(" ");
        }
        printf("*");
        for(int j = 0; j < x - k - 1; j++){
            printf(" ");
        }
        for(int j = 0; j <= 0 + 2 * k; j++){
            printf("*");
        }
        for(int j = 0; j < x / 2 - k; j++){
            printf(" ");
        }
        printf("\n");
    }
    for(int i = 0; i <= x * 2; i++){
        printf("*");
    }
    printf("\n");
    for(int k = 0; k <= x / 2; k++){
        for(int j = x / 2 - k; j <= x / 2; j++){
            printf(" ");
        }
        for(int j = x - 2 * k; j >= 1; j--){
            printf("*");
        }
        for(int j = x; j < x + (x / 2) + k; j++){
            printf(" ");
        }
        printf("*");
        printf("\n");
    }

}
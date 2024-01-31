#include<stdio.h>

int arrow(int size){
    for(int k = 0; k < size / 2; k++){
        for(int j = 0; j < size / 2; j++){
            printf(" ");
        } //Aqui esta bien
        printf("*");//Aqui esta bien
        for(int j = 0; j < (size - k)-1; j++){
            printf(" ");
        }//Aqui ya esta bien
        for(int j = 0; j <=(2 * k); j++){
            printf("*");
        }
        /*for(int j = 0; j < size / 2 - k; j++){
            printf(" ");
        }*/
        printf("\n");//Parte de arriba completa
    }
    for(int i = 1; i <= size * 2; i++){
        printf("*");
    }//Parte de enmedio completa
    
    printf("\n");
    for(int k = 0; k < size / 2; k++){
        for(int j = size / 2 - k; j <= size / 2; j++){
            printf(" ");
        }
        for(int j = size-(2 * k)-2; j >= 1; j--){
            printf("*");
        }
        for(int j = size; j < size + (size / 2) + k + 1; j++){
            printf(" ");
        }
        printf("*");
        printf("\n");
    }
}

int main(){
    int size;
    char rand[20];
    printf("Inserta el tamanio de la base de la felcha: ");
    scanf("%d", &size);
    if(size % 2 == 0){
        size--;
        printf("Se va a reducir el show\n");
    }
    arrow(size);
}
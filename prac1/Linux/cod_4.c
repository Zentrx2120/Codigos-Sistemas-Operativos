#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

    char x[100];

    printf("Ingresar la cadena a comprobar\n");
    fgets(x, sizeof(x), stdin);

    //printf("\nLa cadena es la siguiente: \n%s\n", x);

    if(strchr(x,'(') != 0){
        //printf("Aqui huele a obo\n");
        if(strchr(x,')')!=NULL){
            printf("Todo bien\n");
        }else{
            printf("No cerro el parentecis\n");
        }
    }

    if(strchr(x,'{') != 0){
        //printf("Aqui huele a obo\n");
        if(strchr(x,'}')!=NULL){
            printf("Todo bien\n");
        }else{
            printf("No cerro las llaves\n");
        }
    }

    if(strchr(x,'[') != 0){
        //printf("Aqui huele a obo\n");
        if(strchr(x,']')!=NULL){
            printf("Todo bien\n");
        }else{
            printf("No cerro el corchete\n");
        }
    }

    return 0;
}
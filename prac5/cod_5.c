#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(){
    pid_t pid;
    char *argv[3];

    argv[0]="/home/sebastian/Escritorio/SisOp/Codigos/prac5/hola";
    argv[1]="Desde su hijo";
    argv[2]=NULL;

    if((pid=fork())==0){
        printf("Soy el hijo ejecutado: %c\n");
        execv(argv[0],argv);
    }
    else{
        wait(0);
        printf("Soy el pradre\n");
        exit(0);
    }

}
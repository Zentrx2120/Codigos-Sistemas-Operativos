#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid, pidh;
    int x, i, y;
    pid = fork();
    if (pid == -1)
    {
        perror("Error al crear el proceso hijo");
        exit(EXIT_FAILURE);
    }
    if (pid == 0)
    {
        printf("Soy el hijo %d, mi padre es %d\n", getpid(), getppid());
        pid = fork();
        if (pid == -1){
            perror("Error al crear el proceso hijo");
            exit(EXIT_FAILURE);
        }
        if (pid == 0){
            printf(" Soy el hijo %d, mi padre es %d\n", getpid(), getppid());
            for (x = 1; x <= 5; x++){
                pid = fork();
                if (pid == -1){
                    perror("Error al crear el proceso");
                    exit(EXIT_FAILURE);
                }
            switch (x){
            case 1:
                if (pid == 0){
                    printf(" Soy el hijo %d, mi padre es %d\n", getpid(), getppid());
                // Cada hijo tiene un solo hijo
                    for (i = 0; i < 5; i++){
                        pid = fork();
                        if (pid == -1){
                            perror("Error al crear el proceso hijo");
                            exit(EXIT_FAILURE);
                        }
                        if (pid == 0){
                            printf(" Soy el hijo %d, mi padre es %d\n", getpid(), getppid());
                            if(i==4){
                                for (y = 0; y < 5; y++){
                                    pidh = fork();
                                    if (pidh == -1){
                                        perror("Error al crear el proceso hijo");
                                        exit(EXIT_FAILURE);
                                    }
                                    if (pidh == 0){
                                        printf(" Soy el hijo %d, mi padre es %d\n", getpid(), getppid());
                                        exit(0);
                                    }
                                    else{
                                    // El proceso hijo espera a que el nieto termine
                                        wait(NULL);}
                                }
                            }
                        }
                        else{
                        // El proceso hijo espera a que el nieto termine
                            wait(NULL);
                            break;
                        }
                    }// Salir del bucle después de crear los hijos
                    exit(0);
                }
                else{
// El proceso padre espera a que el hijo termine
                    wait(NULL);
                }break;
            case 2:
            case 3:
                if (pid == 0){
                    printf(" Soy el hijo %d, mi padre es %d\n", getpid(), getppid());
// Cada hijo tiene un solo hijo
                    for (i = 0; i < 2; i++){
                        pid = fork();
                        if (pid == -1){
                            perror("Error al crear el proceso hijo");
                            exit(EXIT_FAILURE);
                        }
                        if (pid == 0){
                            printf(" Soy el hijo %d, mi padre es %d\n", getpid(), getppid());
                            if(i==1){                                    
                                for (y = 0; y < 3; y++){
                                    pidh = fork();
                                    if (pidh == -1){
                                        perror("Error al crear el proceso hijo");
                                        exit(EXIT_FAILURE);
                                    }
                                    if (pidh == 0){
                                        printf(" Soy el hijo %d, mi padre es %d\n", getpid(), getppid());
                                        exit(0);
                                    }
                                    else{
// El proceso hijo espera a que el nieto termine
                                        wait(NULL);
// Importante: el proceso hijo debe salir del bucle para evitar crear más nietos.
                                    }
                                }
                            }
                        }
                        else{
// El proceso hijo espera a que el nieto termine
                            wait(NULL);
                            break;
                        }
                    }// Salir del bucle después de crear los hijos
                    exit(0);
                }
                else{
                    wait(NULL);
                }
                break;
                case 4:
                case 5:
                    if (pid == 0){
                        printf(" Soy el hijo %d, mi padre es %d\n", getpid(), getppid());
// Cada hijo tiene un solo hijo
                        pid = fork();
                        if (pid == -1){
                            perror("Error al crear el proceso hijo");
                            exit(EXIT_FAILURE);
                        }
                        if (pid == 0){
                            printf(" Soy el hijo %d, mi padre es %d\n", getpid(), getppid());
                            for (y = 0; y < 2; y++){
                                pidh = fork();
                                if (pidh == -1){
                                    perror("Error al crear el proceso hijo");
                                    exit(EXIT_FAILURE);
                                }
                                if (pidh == 0){
                                    printf(" Soy el hijo %d, mi padre es %d\n", getpid(), getppid());
                                    exit(0);
                                }
                                else{
// El proceso hijo espera a que el nieto termine
                                    wait(NULL);
                                }
                            }
                        }
                        else{
// El proceso hijo espera a que el nieto termine
                            wait(NULL);
                            exit(0);
                        }
// Salir del bucle después de crear los hijos
                        exit(0);
                    }
                    else{
// El proceso padre espera a que el hijo termine
                        wait(NULL);
                    }
                    break;
                }
            }
        }
        else{
// El proceso hijo espera a que el nieto termine
            wait(NULL);
        }
    }
    else{
// El proceso padre espera a que el hijo termine
        wait(NULL);
    }
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(){
    pid_t id_proc;
    int i;
    int status;
    char *argv0[2], *argv1[2], *argv2[2];

    // EL PROCESO PADRE CREA UN HIJO
    id_proc = fork();
    if (id_proc == 0){
        printf("Soy el primer hijo con PID %d, creando tres hijos mas\n", getpid());

    // Crear tres hijos mas del primer hijo
        for (i = 0; i < 3; i++){
            pid_t child_pid = fork();

            if (child_pid == 0) {
            // Código de los tres nuevos hijos
                argv0[0] = "/home/sebastian/Escritorio/SisOp/Codigos/prac5/permisos";
                argv1[0] = "/home/sebastian/Escritorio/SisOp/Codigos/prac5/aritmetica";
                argv2[0] = "/home/sebastian/Escritorio/SisOp/Codigos/prac5/cod_1";
                argv0[1] = NULL;
                argv1[1] = NULL;
                argv2[1] = NULL;

                switch (i) {
                    case 0:
                        printf("\nSoy el hijo %d del primer hijo con PID %d ejecutando %s\n", i + 1, getpid(), argv0[0]);
                        execv(argv0[0], argv0);
                        break;

                    case 1:
                        printf("\nSoy el hijo %d del primer hijo con PID %d ejecutando %s\n", i + 1, getpid(), argv1[0]);
                        execv(argv1[0], argv1);
                        break;

                    case 2:
                        printf("\nSoy el hijo %d del primer hijo con PID %d ejecutando %s\n", i + 1, getpid(), argv2[0]);
                        execv(argv2[0], argv2);
                        break;

                    default:
                    break;
                }
                exit(0);
            }
        wait(0);
        }
        // El primer hijo espera a que sus tres hijos terminen

        for (i = 0; i < 3; i++){
            wait(0);
        }
        printf("Soy el primer hijo con PID %d, terminando\n", getpid());
        exit(0);
        }
        else if(id_proc > 0){
        // Código del proceso padre
            printf("Soy el proceso padre con PID %d, esperando a que mi hijo termine\n",
            getpid());
            wait(0);
            printf("Soy el proceso padre con PID %d, terminando\n", getpid());
        }
        else {
        // Error al crear el hijo
            fprintf(stderr, "Error al crear el hijo\n");
            exit(1);
        }
    return 0;
}
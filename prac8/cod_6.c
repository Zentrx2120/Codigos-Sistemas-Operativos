#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>

#define N 10
#define ELEMENT_SIZE sizeof(int)

//Definimos el tamanio en bytes de la memoria 
#define SHARED_MEMORY_SIZE 800

// Función para generar una matriz aleatoria
void generarAleatoria(double arr[][N], int filas);

// Función para imprimir una matriz
void imprimirMatriz(double matriz[N][N], int filas, int columnas);

// Función para aplicar eliminación gaussiana a una matriz
void eliminacionGaussiana(double matriz[N][N]);

// Función para aplicar el método de Gauss-Jordan a una matriz y opcionalmente escribir en un archivo
void gaussJordan(double matriz[N][N], FILE *archivo);

int main(void)
{
    // Declaración de variables
    int i, j, shmidA, shmidB, shmidR;
    double (*matrizA)[N], (*matrizB)[N], (*matrizR)[N];
    char nombre_suma[50] = "Resultados_totales_memorias_II.txt";
    FILE *fichero;

    // Borrar el archivo si existe
    remove(nombre_suma);

    // Generar claves únicas para las áreas de memoria compartida
    key_t keyA = ftok(".", 'A');
    key_t keyB = ftok(".", 'B');
    key_t keyR = ftok(".", 'R');

    // Crear áreas de memoria compartida
    shmidA = shmget(keyA, SHARED_MEMORY_SIZE, IPC_CREAT | 0666);
    shmidB = shmget(keyB, SHARED_MEMORY_SIZE, IPC_CREAT | 0666);
    shmidR = shmget(keyR, SHARED_MEMORY_SIZE, IPC_CREAT | 0666);

    // Adjuntar matrices a las áreas de memoria compartida
    matrizA = shmat(shmidA, NULL, 0);
    matrizB = shmat(shmidB, NULL, 0);
    matrizR = shmat(shmidR, NULL, 0);

    srand(time(NULL)); // Inicializar semilla para números aleatorios

    // Generar matrices aleatorias A y B
    generarAleatoria(matrizA, N);
    generarAleatoria(matrizB, N);

    // Copiar matrices A y B a las áreas de memoria compartida
    memcpy(matrizR, matrizA, SHARED_MEMORY_SIZE);
    memcpy((double*)((char*)matrizR + SHARED_MEMORY_SIZE), matrizB, SHARED_MEMORY_SIZE);

    // Proceso hijo 1
    if (fork() == 0){
        // Realizar la suma de matrices en el proceso hijo
        for (i = 0; i < N; i++){
            for (j = 0; j < N; j++){
                matrizR[i][j] += matrizB[i][j];
            }
        }
        // Salir sin imprimir resultados
        exit(0);
    }

    // Proceso hijo 2 (nieto)
    if (fork() == 0){
        // Realizar la multiplicación de matrices en el proceso hijo (nieto)
        for (i = 0; i < N; i++){
            for (j = 0; j < N; j++){
                matrizR[i][j] *= 10.0;
            }
        }
        // Salir sin imprimir resultados
        exit(0);
    }
    // Esperar a que el hijo termine la suma
    wait(NULL);
    wait(NULL);

    // Imprimir resultados en consola solo en el proceso padre
    printf("\nEl resultado de la suma hecha por el hijo es:\n");
    imprimirMatriz(matrizR, N, N);

    // Calcular la inversa de result_suma y result_multp en el proceso padre
    printf("Inversa de la suma:");
    gaussJordan(matrizR, NULL);

    printf("\nEl resultado de la multiplicación hecho por el hijo del hijo (nieto) es:\n");
    imprimirMatriz(matrizR, N, N);

    printf("\nInversa de la multiplicacion:");
    gaussJordan(matrizR, NULL);

    // Abrir archivo en modo escritura (creará uno nuevo si no existe)
    fichero = fopen(nombre_suma, "a+");
    printf("\nFichero: %s -> ", nombre_suma);

    if (fichero)
    {
        printf("\ncreado (ABIERTO)\n");

        // Escritura del resultado de la suma en el archivo
        fputs("\n Resultado Inversa Suma:\n\n", fichero);
        for (i = 0; i < 10; i++)
        {
            for (j = 0; j < 10; j++)
            {
                fprintf(fichero, "    %.2f", (matrizR[i][j]));
            }
            fprintf(fichero, "\n");
        }

        // Escritura del resultado de la multiplicación en el archivo
        fputs("\n Resultado Inversa Multiplicacion:\n\n", fichero);
        for (i = 0; i < 10; i++)
        {
            for (j = 0; j < 10; j++)
            {
                fprintf(fichero, "    %.2f", (matrizR[i][j]));
            }
            fprintf(fichero, "\n");
        }

        // Escritura de la inversa de la suma en el archivo
        fputs("\n Resultado Suma:\n\n", fichero);
        gaussJordan(matrizR, fichero);

        // Escritura de la inversa de la multiplicación en el archivo
        fputs("\n Resultado Multiplicacion:\n\n", fichero);
        gaussJordan(matrizR, fichero);
        // Cerrar archivo
        fclose(fichero);
        printf("\nFichero cerrado\n");
    }
    else
    {
        printf("Error (NO ABIERTO)\n");
    }

    // Desvincular las áreas de memoria compartida
    shmdt(matrizA);
    shmdt(matrizB);
    shmdt(matrizR);

    // Liberar las áreas de memoria compartida
    shmctl(shmidA, IPC_RMID, NULL);
    shmctl(shmidB, IPC_RMID, NULL);
    shmctl(shmidR, IPC_RMID, NULL);

    return 0;
}

void generarAleatoria(double arr[][N], int filas)
{
    // Llenar la matriz con números aleatorios entre 1 y 3
    int i, j;
    for (i = 0; i < filas; i++)
    {
        for (j = 0; j < N; j++)
        {
            arr[i][j] = (rand() % 3) + 1;
        }
    }
}

void imprimirMatriz(double matriz[N][N], int filas, int columnas)
{
    // Imprimir la matriz en consola
    int i, j;
    for (i = 0; i < filas; i++)
    {
        for (j = 0; j < columnas; j++)
        {
            printf("    %.2f", matriz[i][j]);
        }
        printf("\n");
    }
}
void eliminacionGaussiana(double matriz[N][N])
{
    // Aplicar eliminación gaussiana a la matriz
    for (int i = 0; i < N; i++)
    {
        double divisor = matriz[i][i];
        for (int j = 0; j < N; j++)
        {
            matriz[i][j] /= divisor;
        }

        for (int k = 0; k < N; k++)
        {
            if (k != i)
            {
                double factor = matriz[k][i];
                for (int j = 0; j < N; j++)
                {
                    matriz[k][j] -= factor * matriz[i][j];
                }
            }
        }
    }
}

void gaussJordan(double matriz[N][N], FILE *archivo){
    // Crear una matriz extendida para almacenar la inversa
    double extendida[N][2 * N];

    // Inicializar la matriz extendida
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            extendida[i][j] = matriz[i][j];
            extendida[i][j + N] = (i == j) ? 1.0 : 0.0;
        }
    }
    // Aplicar eliminación gaussiana y gauss-jordan a la matriz extendida
    for (int i = 0; i < N; i++)
    {
        double divisor = extendida[i][i];
        for (int j = 0; j < 2 * N; j++){
            extendida[i][j] /= divisor;
        }
    // Aplicar eliminación gaussiana y gauss-jordan a la matriz extendida
        for (int i = 0; i < N; i++){
            double divisor = extendida[i][i];
            for (int j = 0; j < 2 * N; j++){
                extendida[i][j] /= divisor;
            }

            for (int k = 0; k < N; k++){
                if (k != i)
                {
                    double factor = extendida[k][i];
                    for (int j = 0; j < 2 * N; j++){
                        extendida[k][j] -= factor * extendida[i][j];
                    }
                }
            }
        }

        // Extraer la matriz inversa de la parte derecha de la matriz extendida
        for(int i = 0; i < N; i++){
            for (int j = 0; j < N; j++){
                matriz[i][j] = extendida[i][j + N];
            }
        }

        // Imprimir la matriz inversa en la consola
        printf("\n");
        imprimirMatriz(matriz, N, N);

        // Si se proporciona un archivo, escribir la matriz inversa en el archivo
        if (archivo != NULL){
            fprintf(archivo, "\n");
            for (int i = 0; i < N; i++){
                for (int j = 0; j < N; j++){
                    fprintf(archivo, "%.2f ", matriz[i][j]);
                }
                fprintf(archivo, "\n");
            }
        }
    }
}
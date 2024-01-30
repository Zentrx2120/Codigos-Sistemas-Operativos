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
    int i, j, shmidA, shmidB, shmidC, shmidD, shmidSuma, shmidMultp;
    double (*matrizA)[N], (*matrizB)[N], (*matrizC)[N], (*matrizD)[N], (*suma)[N], (*multi)[N];
    double (*result_suma)[N], (*result_multp)[N];
    char nombre_suma[50] = "Resultados_totales_memorias.txt";
    FILE *fichero;

    // Borrar el archivo si existe
    remove(nombre_suma);

    // Generar claves únicas para las áreas de memoria compartida
    key_t keyA = ftok(".", 'A');
    key_t keyB = ftok(".", 'B');
    key_t keyC = ftok(".", 'C');
    key_t keyD = ftok(".", 'D');
    key_t keySuma = ftok(".", 'E');
    key_t keyMultp = ftok(".", 'F');

    // Crear áreas de memoria compartida
    shmidA = shmget(keyA, SHARED_MEMORY_SIZE, IPC_CREAT | 0666);
    shmidB = shmget(keyB, SHARED_MEMORY_SIZE, IPC_CREAT | 0666);
    shmidC = shmget(keyC, SHARED_MEMORY_SIZE, IPC_CREAT | 0666);
    shmidD = shmget(keyD, SHARED_MEMORY_SIZE, IPC_CREAT | 0666);
    shmidSuma = shmget(keySuma, SHARED_MEMORY_SIZE, IPC_CREAT | 0666);
    shmidMultp = shmget(keyMultp, SHARED_MEMORY_SIZE, IPC_CREAT | 0666);
    // Adjuntar matrices a las áreas de memoria compartida
    matrizA = shmat(shmidA, NULL, 0);
    matrizB = shmat(shmidB, NULL, 0);
    matrizC = shmat(shmidC, NULL, 0);
    matrizD = shmat(shmidD, NULL, 0);
    suma = shmat(shmidSuma, NULL, 0);
    multi = shmat(shmidMultp, NULL, 0);

    srand(time(NULL)); // Inicializar semilla para números aleatorios

    // Generar matrices aleatorias A y B
    generarAleatoria(matrizA, N);
    generarAleatoria(matrizB, N);

    // Copiar matrices A y B a las áreas de memoria compartida
    memcpy(matrizC, matrizA, sizeof(double[N][N]));
    memcpy(matrizD, matrizB, sizeof(double[N][N]));

    // Proceso hijo 1
    if (fork() == 0)
    {
        // Realizar la suma de matrices en el proceso hijo
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                suma[i][j] = (matrizC[i][j] + matrizD[i][j]);
            }
        }
        // Salir sin imprimir resultados
        exit(0);
    }
    // Esperar a que el hijo termine la suma
    wait(NULL);

    // Proceso hijo 2 (nieto)
    if (fork() == 0)
    {
        // Leer matrices C y D desde las áreas de memoria compartida
        memcpy(matrizC, matrizA, sizeof(double[N][N]));
        memcpy(matrizD, matrizB, sizeof(double[N][N]));

        // Realizar la multiplicación de matrices en el proceso hijo (nieto)
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                multi[i][j] = (matrizC[i][j] * matrizD[i][j] * 10.0);
            }
        }
        // Salir sin imprimir resultados
        exit(0);
    }
// Esperar a que el hijo del hijo termine la multiplicación
    wait(NULL);

    // Imprimir resultados en consola solo en el proceso padre
    printf("\nEl resultado de la suma hecha por el hijo es:\n");
    imprimirMatriz(suma, N, N);

    printf("\nEl resultado de la multiplicación hecho por el hijo del hijo (nieto) es:\n");
    imprimirMatriz(multi, N, N);

    // Calcular la inversa de result_suma y result_multp en el proceso padre
    printf("\nInversas de las matrices calculadas por el padre/abuelo\n");
    printf("Inversa de la suma:");
    gaussJordan(suma, NULL);

    printf("\nInversa de la multiplicacion:");
    gaussJordan(multi, NULL);

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
                fprintf(fichero, "    %.2f", (suma[i][j]));
            }
            fprintf(fichero, "\n");
        }

        // Escritura del resultado de la multiplicación en el archivo
        fputs("\n Resultado Inversa Multiplicacion:\n\n", fichero);
        for (i = 0; i < 10; i++)
        {
            for (j = 0; j < 10; j++)
            {
                fprintf(fichero, "    %.2f", (multi[i][j]));
            }
            fprintf(fichero, "\n");
        }

        // Escritura de la inversa de la suma en el archivo
        fputs("\n Resultado Suma:\n\n", fichero);
        gaussJordan(suma, fichero);

        // Escritura de la inversa de la multiplicación en el archivo
        fputs("\n Resultado Multiplicacion:\n\n", fichero);
        gaussJordan(multi, fichero);
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
    shmdt(matrizC);
    shmdt(matrizD);
    shmdt(suma);
    shmdt(multi);

    // Liberar las áreas de memoria compartida
    shmctl(shmidA, IPC_RMID, NULL);
    shmctl(shmidB, IPC_RMID, NULL);
    shmctl(shmidC, IPC_RMID, NULL);
    shmctl(shmidD, IPC_RMID, NULL);
    shmctl(shmidSuma, IPC_RMID, NULL);
    shmctl(shmidMultp, IPC_RMID, NULL);

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
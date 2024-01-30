#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>

#define SIZE 10

//Funcion para generar una matriz aleatoria en un rango de 0 a 10
void generarMatrizAleatoria(int matriz[SIZE][SIZE]){
    for (int i = 0; i< SIZE; ++i){
        for (int j = 0; j < SIZE; ++j){
            matriz[i][j]= rand() % 11;
        }
    }
}
//-----------------------------------------------------------------------------

void imprimirMatriz(int matriz[SIZE][SIZE]){
    for (int i = 0; i < SIZE; ++i){
        for (int j = 0; j < SIZE; ++j){
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
}
//-----------------------------------------------------------------------------

void sumarMatrices(int A[SIZE][SIZE], int B[SIZE][SIZE], int resultado[SIZE][SIZE]){
    for (int i = 0; i < SIZE; ++i){
        for (int j = 0; j < SIZE; ++j){
            resultado[i][j] = A[i][j] + B[i][j];
        }
    }
    escribirArchivo(resultado, "suma.txt");
}
//------------------------------------------------------------------------------

void restarMatrices(int A[SIZE][SIZE], int B[SIZE][SIZE], int resultado[SIZE][SIZE]){
    for (int i = 0; i < SIZE; ++i){
        for (int j = 0; j < SIZE; ++j){
            resultado[i][j] = A[i][j] - B[i][j];
        }
    }
    escribirArchivo(resultado, "resta.txt");
}
//--------------------------------------------------------------------------------

void multiplicarMatrices(int A[SIZE][SIZE], int B[SIZE][SIZE], int resultado[SIZE][SIZE]){
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            resultado[i][j] = 0;
            for (int k = 0; k < SIZE; ++k) {
                resultado[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    escribirArchivo(resultado, "multiplicacion.txt");
}
//------------------------------------------------------------------------------------

void obtenerTranspuesta(int matriz[SIZE][SIZE], int resultado[SIZE][SIZE]){
    for (int i = 0; i < SIZE; ++i){
        for (int j = 0; j < SIZE; ++j){
            resultado[j][i] = matriz[i][j];
        }
    }
}
//--------------------------------------------------------------------------------------

void obtenerInversa(int matriz[SIZE][SIZE], gsl_matrix *copiaMatriz, gsl_matrix *matrizInversa, gsl_permutation *permutacion, int signo){
// Crear y asignar valores a la matriz GSL
    matrizA_a_gsl_matrix(matriz, copiaMatriz);
    gsl_linalg_LU_decomp(copiaMatriz, permutacion, &signo);
    gsl_linalg_LU_invert(copiaMatriz, permutacion, matrizInversa);
}
//--------------------------------------------------------------------------------------

void escribirArchivoInversa(gsl_matrix *resultado, const char *nombreArchivo){
    FILE *archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL){
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }
    almacenarMI(resultado, archivo);
    fclose(archivo);
}
//---------------------------------------------------------------------------------------

void almacenarMI(gsl_matrix *matriz, FILE *archivo){
    for (size_t i = 0; i < matriz->size1; ++i){
        for (size_t j = 0; j < matriz->size2; ++j){
            fprintf(archivo, "%g ", gsl_matrix_get(matriz, i, j));
        }
        fprintf(archivo, "\n");
    }
}
//---------------------------------------------------------------------------------------

void leerMI(const char *nombreArchivo){
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL){
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }
    double valor;
    int elementosEnFila = 0;
    while (fscanf(archivo, "%lf", &valor) == 1){
        printf("%g ", valor);
        elementosEnFila++;
        // Agregar un salto de línea después de cada conjunto de 10 elementos
        if (elementosEnFila == 10){
            printf("\n");
            elementosEnFila = 0; // Reiniciar el contador
        }
    }
    fclose(archivo);
}
//---------------------------------------------------------------------------------------

void escribirArchivo(int resultado[SIZE][SIZE], const char *nombreArchivo){
    FILE *archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL){
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }
    almacenarMenA(resultado, archivo);
    fclose(archivo);
}
//---------------------------------------------------------------------------------------

void almacenarMenA(int matriz[SIZE][SIZE], FILE *archivo){
    for (int i = 0; i < SIZE; ++i){
        for (int j = 0; j < SIZE; ++j){
            fprintf(archivo, "%d ", matriz[i][j]);
        }
        fprintf(archivo, "\n");
    }
}
//---------------------------------------------------------------------------------------

void leerArchivo(const char *nombreArchivo){
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL){
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }
    int valor;
    int elementosEnLinea = 0;
    while (fscanf(archivo, "%d", &valor) == 1){
        printf("%d ", valor);
        elementosEnLinea++;
        // Después de cada 10 elementos, imprimir un salto de línea
        if (elementosEnLinea == 10){
            printf("\n");
            elementosEnLinea = 0; // Reiniciar el contador
        }
    }
    fclose(archivo);
}
//---------------------------------------------------------------------------------------

void matrizA_a_gsl_matrix(int matrizA[SIZE][SIZE], gsl_matrix *matrizGSL){
    for (size_t i = 0; i < SIZE; ++i){
        for (size_t j = 0; j < SIZE; ++j){
            gsl_matrix_set(matrizGSL, i, j, (double)matrizA[i][j]);
        }
    }
}
//---------------------------------------------------------------------------------------

int main(){

    srand(time(NULL));
    int matrizA[SIZE][SIZE];
    int matrizB[SIZE][SIZE];
    int resultado[SIZE][SIZE];
    gsl_matrix *copiaMatriz = gsl_matrix_alloc(SIZE, SIZE);
    gsl_matrix *copiaMatriz2 = gsl_matrix_alloc(SIZE, SIZE);
    gsl_matrix *matrizInversa = gsl_matrix_alloc(SIZE, SIZE);
    gsl_matrix *matrizInversa2 = gsl_matrix_alloc(SIZE, SIZE);
    gsl_permutation *permutacion = gsl_permutation_alloc(SIZE);
    gsl_permutation *permutacion2 = gsl_permutation_alloc(SIZE);
    int signo;
    clock_t inicio, fin;
    double tiempo;

    inicio = clock();
    // Inicializar las matrices A y B

    generarMatrizAleatoria(matrizA);
    printf("Matriz A: \n");
    imprimirMatriz(matrizA);

    generarMatrizAleatoria(matrizB);
    printf("\nMatriz B: \n");
    imprimirMatriz(matrizB);

    // Creando los procesos
    for (int i = 0; i < 5; ++i){
        pid_t pid = fork();
        if (pid == -1){
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid == 0){
        // Codigo del proceso hijo
            switch (i) {
            case 0:
                sumarMatrices(matrizA, matrizB, resultado);
            break;

            case 1:
                restarMatrices(matrizA, matrizB, resultado);
            break;

            case 2:
                multiplicarMatrices(matrizA, matrizB, resultado);
            break;

            case 3:
                obtenerTranspuesta(matrizA, resultado);
                escribirArchivo(resultado, "transpuesta_A.txt");

                obtenerTranspuesta(matrizB, resultado);
                escribirArchivo(resultado, "transpuesta_B.txt");
            break;

            case 4:
                obtenerInversa(matrizA, copiaMatriz, matrizInversa, permutacion, signo);
                escribirArchivoInversa(matrizInversa, "inversa_A.txt");
                obtenerInversa(matrizB, copiaMatriz2, matrizInversa2, permutacion2, signo);
                escribirArchivoInversa(matrizInversa2, "inversa_B.txt");
            break;

            default:
            break;
            }
        exit(EXIT_SUCCESS);
        }
    }

    // Proceso padre espera a que todos los procesos hijos terminen para poder imprimir los resultados desde el archivo
    for (int i = 0; i < 5; ++i){
        wait(NULL);
    }
    // Proceso sexto lee archivos y muestra en pantalla

    printf("\nEl resultado de la suma de las matrices A y B es: \n");
    leerArchivo("suma.txt");

    printf("\nEl resultado de la resta de las matrices A y B es: \n");
    leerArchivo("resta.txt");

    printf("\nEl resultado de la multiplicación de las matrices A y B es: \n");
    leerArchivo("multiplicacion.txt");

    printf("\nLa matriz transpuesta de A es: \n");
    leerArchivo("transpuesta_A.txt");

    printf("\nLa matriz transpuesta de B es: \n");
    leerArchivo("transpuesta_B.txt");

    printf("\nLa matriz inversa de A es: \n");
    leerMI("inversa_A.txt");

    printf("\nLa matriz inversa de B es: \n");
    leerMI("inversa_B.txt");

    fin = clock();
    tiempo = ((double)(fin - inicio)) / CLOCKS_PER_SEC;
    printf("\nTiempo de ejecución: %f segundos\n", tiempo);return 0;
}
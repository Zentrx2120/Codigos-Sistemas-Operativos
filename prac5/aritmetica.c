#include <stdio.h>

// Función para evaluar la ecuación aritmética
float evaluarEcuacion(char ecuacion[]) {
    float resultado;
    sscanf(ecuacion, "%f", &resultado);
    return resultado;
}

int main() {
    char ecuacion[100];

    // Solicitar al usuario ingresar la ecuación
    printf("Ingrese una ecuación aritmética: ");
    fgets(ecuacion, sizeof(ecuacion), stdin);

    // Eliminar el salto de línea del final de la cadena
    int i = 0;
    while (ecuacion[i] != '\n' && ecuacion[i] != '\0') {
        i++;
    }
    ecuacion[i] = '\0';

    // Evaluar y mostrar el resultado
    float resultado = evaluarEcuacion(ecuacion);
    printf("El resultado es: %.2f\n", resultado);

    return 0;
}
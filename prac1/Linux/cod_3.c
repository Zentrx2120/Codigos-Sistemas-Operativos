#include <stdio.h>
#define disco 3

void torres(int, char, char, char);

int main()
{
    torres(disco, '1', '2', '3');
    printf("\n");
    return 0;
}

//Funcion principal--------------------------------------------------------
void torres(int num, char pos, char arriba, char aux)
{
    if (num == 1){
        printf("\n Disco 1 del poste %c al poste %c\n", pos, arriba);
        return;
    }
    
    if (num > 1) {
        torres(num - 1, pos, aux, arriba);
        printf("\n Disco %d del poste %c al poste %c\n", num, pos, arriba);
        torres(num - 1, aux, arriba, pos);
    }
}
//Aquí queremos generar matrices de n*n de enteros con números aleatorios con valores de lower a upper definidos internamente
//El programa recibe un valor entero entregado por el usuario que corresponde al tamaño de la matriz cuadrada
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
int main()
{
  int size, i, j, lower=10, upper=30, r;
  /* Inicializa el generador de números aleatorios */
  srand(time(NULL));
  printf("Por favor ingrese el tamaño de la matriz cuadrada: ");
  scanf("%d", &size);
  int matrix[size][size];
  for (i=0; i<size; i++) {
    for (j=0; j<size; j++) {
      r = (rand() % (upper - lower + 1)) + lower;
      matrix[i][j] = r;
    }
  }
    
  printf("Esta es la matriz:\n");
  for (i=0; i<size; i++) {
    for (j=0; j<size; j++) {
      printf("%d ", matrix[i][j]);
    }
    printf("\n");
  }
  return 0;
}

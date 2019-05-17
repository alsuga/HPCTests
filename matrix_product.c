//Aquí se solicita al usuario un tamaño de las matrices, luego se llenan con valores aleatorios de un lower a un upper y luego se realiza la multiplicación
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
int main()
{
  int size, i, j, k, lower=0, upper=10, rA, rB;
  /* Inicializa el generador de números aleatorios */
  srand(time(NULL));
  printf("Por favor ingrese el tamaño de las matrices cuadradas: ");
  scanf("%d", &size);
  int matrixA[size][size];
  int matrixB[size][size];
  int matrixP[size][size];
  for (i=0; i<size; i++) {
    for (j=0; j<size; j++) {
      rA = (rand() % (upper - lower + 1)) + lower;
      rB = (rand() % (upper - lower + 1)) + lower;
      matrixA[i][j] = rA;
      matrixB[i][j] = rB;
    }
  }
    
  printf("Esta es la matrizA:\n");
  for (i=0; i<size; i++) {
    for (j=0; j<size; j++) {
      printf("%d ", matrixA[i][j]);
    }
    printf("\n");
  }

  printf("Esta es la matrizB:\n");
  for (i=0; i<size; i++) {
    for (j=0; j<size; j++) {
      printf("%d ", matrixB[i][j]);
    }
    printf("\n");
  }

  /*Cálculo del producto*/


  printf("Este es el producto de las matrices:\n");
  for (i=0; i<size; i++) {
    for (j=0; j<size; j++) {
      matrixP[i][j]=0;
      for (k=0; k<size; k++) {
        matrixP[i][j] += matrixA[i][k] * matrixB[k][j];
      }
    }
  }

  printf("Este es el producto:\n");
  for (i=0; i<size; i++) {
    for (j=0; j<size; j++) {
      printf("%d ", matrixP[i][j]);
    }
    printf("\n");
  }


  return 0;
}


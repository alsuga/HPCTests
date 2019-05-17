//Aquí se solicita al usuario un tamaño de las matrices, luego se llenan con valores aleatorios de un lower a un upper y luego se realiza la multiplicación usando hilos
//Se usa la cabecera pthread para manejar los hilos, se crea luego una estructura para los parametros que se pasarán a cada hilo
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


#define SIZE 5
int matrixA[SIZE][SIZE];
int matrixB[SIZE][SIZE];
int matrixP[SIZE][SIZE];

struct parameters {
  int num_row;
  int num_col;
};

void *dotproduct(void *arg) {
  struct parameters *p;
  int k;
  p = (struct parameters *)arg;
  //Fila multiplicado por columna (producto punto)
  matrixP[p->num_row][p->num_col]=0;
  for (k=0; k<SIZE; k++) {
    matrixP[p->num_row][p->num_col]+=matrixA[p->num_row][k]*matrixB[k][p->num_col];
  }
  return(NULL);
}

int main()
{
  int i, j, k, lower=0, upper=10, rA, rB;
  /* Inicializa el generador de números aleatorios */
  srand(time(NULL));
  //Se llenan las matricres A y B con números aleatorios
  for (i=0; i<SIZE; i++) {
    for (j=0; j<SIZE; j++) {
      rA = (rand() % (upper - lower + 1)) + lower;
      rB = (rand() % (upper - lower + 1)) + lower;
      matrixA[i][j] = rA;
      matrixB[i][j] = rB;
    }
  }
    
  printf("Esta es la matrizA:\n");
  for (i=0; i<SIZE; i++) {
    for (j=0; j<SIZE; j++) {
      printf("%d ", matrixA[i][j]);
    }
    printf("\n");
  }

  printf("Esta es la matrizB:\n");
  for (i=0; i<SIZE; i++) {
    for (j=0; j<SIZE; j++) {
      printf("%d ", matrixB[i][j]);
    }
    printf("\n");
  }

  /*Cálculo del producto*/
  pthread_t h[SIZE][SIZE];
  struct parameters p[SIZE][SIZE];
  for (i=0; i<SIZE; i++) {
    for (j=0; j<SIZE; j++) {
      p[i][j].num_row=i;
      p[i][j].num_col=j;
      pthread_create(&h[i][j],NULL,dotproduct,( void *)&p[i][j]);
      pthread_join(h[i][j],NULL);
    }
  }
  printf("Este es el producto:\n");
  for (i=0; i<SIZE; i++) {
    for (j=0; j<SIZE; j++) {
      printf("%d ", matrixP[i][j]);
    }
    printf("\n");
  }

  return 0;
}


//Aquí se solicita al usuario un tamaño de las matrices, luego se llenan con valores aleatorios de un lower a un upper y luego se realiza la multiplicación usando hilos
//Se usa la cabecera pthread para manejar los hilos, se crea luego una estructura para los parametros que se pasarán a cada hilo
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


#define SIZE 700
int matrixA[SIZE][SIZE];
int matrixB[SIZE][SIZE];
int matrixP[SIZE][SIZE];

struct parameters {
  int id;
};

void *dotproduct(void *arg) {
  struct parameters *p;
  int i, j, k;
  p = (struct parameters *)arg;
  if (p->id==1) {
    for (i=0; i<(SIZE/2); i++) {
      for (j=0; j<SIZE; j++) {
        matrixP[i][j]=0;
        for (k=0; k<SIZE; k++) {
          matrixP[i][j] += matrixA[i][k] * matrixB[k][j];
        }
      }
    }
  }
  else {
    for(i=(SIZE/2);i<SIZE;i++) {
      for(j=0;j<SIZE;j++) {
        matrixP[i][j]=0;
        for (k=0; k<SIZE; k++) {
          matrixP[i][j] += matrixA[i][k] * matrixB[k][j];
        }
      }
    }
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
  clock_t t; 
  t = clock();
  // pthread_t h[SIZE][SIZE];
  pthread_t h1;
  pthread_t h2;
  // struct parameters p[SIZE][SIZE];
  struct parameters p1;
  struct parameters p2;
  p1.id=1;
  p2.id=2;
  // for (i=0; i<SIZE; i++) {
  //   for (j=0; j<SIZE; j++) {
  //     p[i][j].num_row=i;
  //     p[i][j].num_col=j;
  //     pthread_create(&h[i][j],NULL,dotproduct,( void *)&p[i][j]);
  //     pthread_join(h[i][j],NULL);
  //   }
  // }
  pthread_create(&h1,NULL,dotproduct,( void *)&p1);
  pthread_create(&h2,NULL,dotproduct,( void *)&p2);
  pthread_join(h1,NULL);
  pthread_join(h2,NULL);
  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

  printf("Este es el producto:\n");
  for (i=0; i<SIZE; i++) {
    for (j=0; j<SIZE; j++) {
      printf("%d ", matrixP[i][j]);
    }
    printf("\n");
  }
  printf("Se demoró %f segundos para ejecutarse\n", time_taken); 
  return 0;
}


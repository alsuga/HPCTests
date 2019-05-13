#include <stdio.h>
#include <stdlib.h>

void mat_mult(double da[], double db[], double out[], int m);
 
int main()
{
  int m = 5;
  //scanf("%d", &m);
  double da[m * m];
  double db[m * m];
  double out[m * m];
 
  for(int i = 0; i < m * m; i++) {
      //da[i] = rand() % 100;
      //db[i] = rand() % 100;
      da[i] = 1;
      db[i] = 1;
    }

  mat_mult(da, db, out, m);

  for(int i = 0; i < m; i++) {
    for(int j = 0; j < m; j++)
      printf("%f ", out[m * i + j]);
    printf("\n");
  }
    
	//matrix_t a = { 4, 4, da }, b = { 4, 3, db };
	//matrix c = mat_mul(&a, &b);
 
	/* mat_show(&a), mat_show(&b); */
	//mat_show(c);
	/* free(c) */
	return 0;
}


void mat_mult(double da[], double db[], double out[], int m) {
  int indiceRespuesta;
  int indiceMatrizA;
  int indiceMatrizB;
  for(int i = 0; i < m; i++)
    for(int j = 0; j < m; j++){
      indiceRespuesta = m * i + j;
      out[indiceRespuesta] = 0;
      for(int k = 0; k < m; k++){
        indiceMatrizA = m * i + k;
        indiceMatrizB = m * k + j;
        out[indiceRespuesta] += da[indiceMatrizA] * db[indiceMatrizB]; 
      }
    }
}
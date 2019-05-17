#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <stdbool.h>
#include <sys/types.h> 
#include <unistd.h>
#include <sys/mman.h>

void mat_mult(double da[], double db[], double out[], int m);
void fill_data(double array[], int m, bool only_one);
void process_mat_mult(double da[], double db[], double out[], int m, int num_proc);
 
int main()
{
  int m = 3;
  double da[m * m];
  double db[m * m];
  double out[m * m];

  fill_data(da, m, true);
  fill_data(db, m, true);
  fill_data(out, m, true);

  //mat_mult(da, db, out, m);
  process_mat_mult(da, db, out, m, m);

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
  int outIndex;
  int daIndex;
  int dbIndex;
  for(int i = 0; i < m; i++)
    for(int j = 0; j < m; j++){
      outIndex = m * i + j;
      out[outIndex] = 0;
      for(int k = 0; k < m; k++){
        daIndex = m * i + k;
        dbIndex = m * k + j;
        out[outIndex] += da[daIndex] * db[dbIndex]; 
      }
    }
}

void process_mat_mult(double da[], double db[], double out[], int m, int num_proc)
{
  int outIndex;
  int daIndex;
  int dbIndex;
  int i;

  int status[num_proc];

  pid_t childProcess[num_proc];

  for(int i = 0; i < num_proc; i++)
  {
    if ((childProcess[i] = fork()) < 0)
    {
      printf("error on fork");
      exit(0);
    }
    else if (childProcess[i] == 0)
    {
      for (int j = 0; j < m; j++)
      {
        outIndex = m * i + j;
        out[outIndex] = 0;
        for (int k = 0; k < m; k++)
        {
          daIndex = m * i + k;
          dbIndex = m * k + j;
          out[outIndex] += da[daIndex] * db[dbIndex];
        }
        printf("out index %d %f\n", outIndex, out[outIndex]);
      }
      exit(0);
    }
  }
  for(int t = 0; t < num_proc; t++) {
    waitpid(childProcess[t], &status[t], 0);
  }
}

void fill_data(double array[], int m, bool only_one) {
  for(int i = 0; i <= m * m; i++)
    array[i] = only_one? 1 : rand() % 99;
}
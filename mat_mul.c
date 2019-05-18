#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <stdbool.h>
#include <sys/types.h> 
#include <unistd.h>
#include <sys/mman.h>
#include <sys/mman.h>

void mat_mult(double da[], double db[], double out[], int m);
void fill_data(double array[], int m, bool only_one);
void process_mat_mult(double da[], double db[], double *out, int m, int num_proc);
void *create_shared_memory(size_t size);
 
int main()
{
  int m = 600;
  double da[m * m];
  double db[m * m];
  double out[m * m];
  double *shmem = (double *)create_shared_memory(m * m * sizeof(double));

  fill_data(da, m, true);
  fill_data(db, m, true);
  fill_data(out, m, true);

  //mat_mult(da, db, out, m);
  process_mat_mult(da, db, shmem, m, m);

  printf("%.1f\n", shmem[0]);
  /*
  for(int i = 0; i < m; i++) {
    for(int j = 0; j < m; j++)
      printf("%.1f ", shmem[m * i + j]);
    printf("\n");
  }*/
    
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


void *create_shared_memory(size_t size) {
  // Our memory buffer will be readable and writable:
  int protection = PROT_READ | PROT_WRITE;

  // The buffer will be shared (meaning other processes can access it), but
  // anonymous (meaning third-party processes cannot obtain an address for it),
  // so only this process and its children will be able to use it:
  int visibility = MAP_ANONYMOUS | MAP_SHARED;

  // The remaining parameters to `mmap()` are not important for this use case,
  // but the manpage for `mmap` explains their purpose.
  return mmap(NULL, size, protection, visibility, -1, 0);
} 
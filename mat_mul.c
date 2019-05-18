#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <stdbool.h>
#include <pthread.h>
#include <sys/types.h> 
#include <unistd.h>
#include <sys/mman.h>
#include <sys/mman.h>

void mat_mult(double *da, double *db, double *out, int m);
void fill_data(double *array, int m, bool only_one);
void process_mat_mult(double *da, double *db, double *out, int m, int num_proc);
void *create_shared_memory(size_t size);
void thread_mat_mul(int m);
void *thread_mul(void *arg);
void nthread_mat_mul(int m, int n_threads);
void *nthread_mul(void *arg);

double *da;
double *db;
double *out;

struct parameters {
	int m;
	int i;
};

struct params {
	int m;
	int t_i;
	int n_t;
};

int main(int argc, char *argv[])
{
	//int m = 500;
	//int b = 1;
	int m = atoi(argv[1]);
	int b = atoi(argv[2]);
	da = (double *)malloc(m * m * sizeof(double));
	db = (double *)malloc(m * m * sizeof(double));
	out = (double *)malloc(m * m * sizeof(double));
	double *shmem = (double *)create_shared_memory(m * m * sizeof(double));

	fill_data(da, m, true);
	fill_data(db, m, true);
	fill_data(out, m, true);

	clock_t t = clock(); 
	if(b == 0) {
		process_mat_mult(da, db, shmem, m, m);
	} else if(b == 1) {
		mat_mult(da, db, out, m);
	} else if(b == 2) {
		thread_mat_mul(m);
	} else if(b == 3) {
		nthread_mat_mul(m, 8);
	}
	t = clock() - t;
	double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
	printf("%.4f ", time_taken);
	printf("%.1f ", out[0]);
	return 0;
}


void mat_mult(double *da, double *db, double *out, int m) {
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

void process_mat_mult(double *da, double *db, double *out, int m, int num_proc)
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

void fill_data(double *array, int m, bool only_one) {
	for(int i = 0; i <= m * m; i++)
		array[i] = only_one? 1 : rand() % 99;
}


void *create_shared_memory(size_t size) {
	int protection = PROT_READ | PROT_WRITE;
	int visibility = MAP_ANONYMOUS | MAP_SHARED;

	return mmap(NULL, size, protection, visibility, -1, 0);
} 

void thread_mat_mul(int m) {
	struct parameters p;
	p.m = m;
	pthread_t *h = (pthread_t *)malloc(m * sizeof(pthread_t));
	for(int i = 0; i < m; i++) {
		p.i = i;
		pthread_create(&h[i], NULL, thread_mul, (void *)&p);
		pthread_join(h[i], NULL);    
	}
}


void *thread_mul(void *arg) {
	struct parameters *p;
	p = (struct parameters *)arg;
	int i = p->i;
	int m = p->m;
	int outIndex, daIndex, dbIndex;
	for(int j = 0; j < m; j++){
		outIndex = m * i + j;
		out[outIndex] = 0;
		for(int k = 0; k < m; k++){
			daIndex = m * i + k;
			dbIndex = m * k + j;
			out[outIndex] += da[daIndex] * db[dbIndex]; 
		}
	}
	return (NULL);
}

void nthread_mat_mul(int m, int n_threads) {
	int n_x_thread = m / n_threads;
	struct params p;
	p.m = m;
	p.n_t = n_x_thread;
	pthread_t *h = (pthread_t *)malloc(n_threads * sizeof(pthread_t));
	for(int i = 0; i < n_threads; i++) {
		p.t_i = i;
		pthread_create(&h[i], NULL, nthread_mul, (void *)&p);
	}
	for(int i = 0; i < n_threads; i++) {
		pthread_join(h[i], NULL);    
	}
}

void *nthread_mul(void *arg) {
	struct params *p;
	p = (struct params *)arg;
	int t_i = p->t_i;
	int m = p->m;
	int n_t = p->n_t;
	int outIndex, daIndex, dbIndex;
	for(int i = t_i * n_t; i < t_i * n_t + n_t; i++) {
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
	return (NULL);
}


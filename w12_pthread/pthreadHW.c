#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define NUM_THREADS 8
#define MSIZE 1024

static double getDoubleTime();
void check();
void *thread_function(void *arg);

// srand(time(NULL));
//int a=(rand()%10)+1;
int A[MSIZE][MSIZE];
int B[MSIZE][MSIZE];
int C[MSIZE][MSIZE];
int D[MSIZE][MSIZE];
int main()
{
	int res;
	int j, i;

	srand(time(NULL));
	for (i = 0; i < MSIZE; i++)
	{
		for (j = 0; j < MSIZE; j++)
		{
			A[i][j] = 1; //(rand()%10)+1;
			B[i][j] = 1; //(rand()%10)+1;
		}
	}
	pthread_t a_thread[NUM_THREADS];
	void *thread_result;
	int lots_of_threads;

	double start_time = getDoubleTime();

	for (lots_of_threads = 0; lots_of_threads < NUM_THREADS; lots_of_threads++)
	{

		res = pthread_create(&(a_thread[lots_of_threads]), NULL, thread_function, (void *)(long)lots_of_threads);
		//        sleep(1);
	}
	printf("Waiting for threads to finish...\n");
	for (lots_of_threads = NUM_THREADS - 1; lots_of_threads >= 0; lots_of_threads--)
	{
		res = pthread_join(a_thread[lots_of_threads], &thread_result);
	}
	double finish_time = getDoubleTime();
	printf("All done\n");

	//check();
	printf("Pthread = %d, MSIZE = %d\n", NUM_THREADS, MSIZE);
	printf("Execute Time:  %.3lf ms\n", (finish_time - start_time));
	exit(EXIT_SUCCESS);
}

void *thread_function(void *arg)
{
	int my_number = (long)arg;
	int i, j, k;
	printf("Thread Number[%d]\n", my_number);
	for (i = (MSIZE / NUM_THREADS) * my_number; i < ((MSIZE / NUM_THREADS) * my_number) + (MSIZE / NUM_THREADS); i++)
	{
		for (j = 0; j < MSIZE; j++)
		{
			for (k = 0; k < MSIZE; k++)
			{
				C[i][j] = C[i][j] + A[i][k] * B[k][j];
				//printf("C[%d][%d]:{%d}=A[%d][%d]:{%d}+B[%d][%d]:{%d}\n",i,j,C[i][j],i,j,A[i][j],i,j,B[i][j]);
			}
		}
	}

	pthread_exit(NULL);
}

static double getDoubleTime()
{
	struct timeval tm;
	gettimeofday(&tm, 0);
	return (double)(((double)tm.tv_sec * (double)1000. + (double)(tm.tv_usec)) * (double)0.001);
}

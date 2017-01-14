#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h> 

#define FOR(src) 																		\
					for(int i = 0; i < 10; i++)											\
					{																	\
						fprintf(stdout, "%s Thread. Iteration: %d\n", (src), i + 1); 	\
						sleep(fRand(0, 2));												\
					}

#define ERROR_EXIT 		fprintf(stderr, "Error\n"); \
						return EXIT_FAILURE;		\

typedef struct {
	int a, b, n;
}args_t; 

double fRand(double fMin, double fMax)
{
	double f = rand() / (RAND_MAX + 1.0);
	return fMin + f * (fMax - fMin);
}

int dRand(int dMin, int dMax)
{
	return (rand() % (dMax + 1 - dMin)) + dMin;
}

void * func1(void * arg)
{
	FOR("Child1");
	return NULL;
}

void * func2(void * arg)
{
	args_t* args = ((args_t*)arg);
	int r;
	srand(time(NULL));
	for(int i = 0; i < args->n; i++)
	{
		r = dRand(args->a, args->b);
		if (r == args->n)
		{
			fprintf(stderr,"ERROR\n");
			pthread_exit(NULL);
		}
		fprintf(stdout, "Child2 Thread. Number: %d\n", r);
	}
	return NULL;
}

int main(void)
{

	pthread_t thread1, thread2;
	args_t args = {4, 5, 5};
	
	if(pthread_create(&thread1, NULL, func1, NULL))
	{
		ERROR_EXIT;
	}

	if(pthread_create(&thread2, NULL, func2, &args))
	{
		ERROR_EXIT;		
	}

	if(pthread_join(thread1, NULL))
	{
		ERROR_EXIT;
	}

	if (pthread_join(thread2, NULL))
	{
		ERROR_EXIT;
	}
	fprintf(stderr, "Main Thread. (end-of-all)\n");
	return EXIT_SUCCESS;
}
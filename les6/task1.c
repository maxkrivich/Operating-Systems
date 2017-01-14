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

double fRand(double fMin, double fMax)
{
	double f = rand() / (RAND_MAX + 1.0);
	return fMin + f * (fMax - fMin);
}

void * func(void * arg)
{
	FOR("Child");
	return NULL;
}

int main(void)
{
	pthread_t thread;
	int res;
	res = pthread_create(&thread, NULL, func, NULL);
	if(res != 0)
	{
		fprintf(stderr, "Error\n");
		return EXIT_FAILURE;
	}
	FOR("Main");
	while(1);
	return EXIT_SUCCESS;
}
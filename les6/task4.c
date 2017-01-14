#include <time.h>
#include <stdio.h>
#include <unistd.h> 
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#define ERROR_EXIT 		fprintf(stderr, "Error\n"); \
						return EXIT_FAILURE;		\

typedef unsigned long long ull;
typedef struct { int size; void * arr; } args_t;

// ull *res_glob;

ull binomial(ull m, ull n)
{
	ull r = 1, d = m - n;
	if (d > n) { n = d; d = m - n; }
 
	while (m > n) 
	{
		r *= m--;
		while (d > 1 && ! (r%d) ) r /= d--;
	}
 
	return r;
}

ull catalan(int n)
{
	return n ? 2 * (2 * n - 1) * catalan(n - 1) / (1 + n) : 1;
}

void * thread_prime(void * arg) //dynamic return
{
	args_t * args = (args_t*)arg;
	int n = args->size, size = 0, pr = 2;
	int * res = malloc(n * sizeof(int));
	while(size != n)
	{
		int num = pr++;
		bool flag = 1;
		for(int j = 2; j*j <= num; j++)
			if(!(num % j))
			{
				flag = 0;
				break;
			}
		if(flag)
		{
			res[size++] = num;
			#ifdef DEBUG
				printf("Thread Prime. Number: %d\n", res[size - 1]);
			#endif
		}
	}
	args->arr = res;
	return NULL;
}

void * thread_catalan(void * arg) // global return
{
	int n = 100;
	ull *res_glob = malloc(n * sizeof(ull));
	for(int i = 0; i < n; i++)
	{
		res_glob[i] = catalan(i);
		#ifdef DEBUG
			printf("Thread Catalan. Number: %llu\n", res_glob[i]);
		#endif
	}
	return NULL;
}

int main(void)
{
	int **arr1;
	ull **arr2;
	args_t arg1 = {100, &arr1};
	args_t arg2 = {10, &arr2};
	pthread_t th_prime, th_catalan;

	if(pthread_create(&th_prime, NULL, thread_prime, &arg1))
	{
		ERROR_EXIT;
	}

	if(pthread_create(&th_catalan, NULL, thread_catalan, &arg2))
	{
		ERROR_EXIT;
	}

	if(pthread_join(th_prime, NULL))
	{
		ERROR_EXIT;
	}

	if (pthread_join(th_catalan, NULL))
	{
		ERROR_EXIT;
	}
	fprintf(stdout, "Main Thread. (end-of-all)\n");
	return EXIT_SUCCESS;
}
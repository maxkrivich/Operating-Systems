#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h> 

#define MAX 4

typedef struct {
	char * name;
	char * str;
	int cnt;
} args_t;

void * func(void * arg)
{
	args_t * args = (args_t *) arg;
	for (int i = 0; i < args->cnt; i++)
		fprintf(stdout, "Thread %s. Str %s\t%d\n", args->name, args->str, i + 1);
	return NULL;
}

int main(void)
{	
	args_t arg[] = { {"kok", "lol", 3}, {"vav", "wow", 4}, {"pop", "ini", 5},  {"rer", "cvc", 6} };
	pthread_t thread[MAX];

	for(int i = 0; i < MAX; i++)
	{
		if (pthread_create(&thread[i], NULL, func, &arg[i])) 
		{
			fprintf(stderr, "Error (thread%d)\n", i + 1);
			// return 1;
		}
	}
	while(1);
	return EXIT_SUCCESS;
}
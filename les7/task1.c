#include <time.h>
#include <stdio.h>
#include <unistd.h> 
#include <getopt.h>
#include <stdlib.h>
#include <pthread.h>
#include "colors.h"

int thread_count = 0;
int * glob_arr = NULL;

typedef struct {
	int num;
} args_t;

void parsearg(int, char **);
void * func(void *);
int dRand(int, int);
void * func_detach(void *);
args_t * gen_args(int);


int main(int argc, char ** argv)
{
	parsearg(argc, argv);
	pthread_t * arr = (pthread_t *) malloc(thread_count * sizeof(pthread_t));
	glob_arr = (int*) calloc(thread_count, sizeof(int));
	// args_t arg = {0};
	pthread_attr_t attr;
	pthread_t thread_detach;
	pthread_attr_init (&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	for(int i = 0; i < thread_count; i++)
	{
		if(!i)
			if(pthread_create(&thread_detach, &attr, &func_detach, NULL));
		if(pthread_create(&arr[i], NULL, &func, gen_args(i)));
		// sleep(0.5);
		// arg.num++;
	}
	for(int i = 0; i < thread_count; i++)
		if(pthread_join(arr[i], NULL))
		{
			fprintf(stderr, "Error\n"); 
			return EXIT_FAILURE;	
		}
	pthread_attr_destroy (&attr);
	fprintf(stdout, BOLD("Main Thread. (end-of-all)\n"));
	return EXIT_SUCCESS;
}

void * func(void * arg)
{
	args_t args = *((args_t*) arg);
	fprintf(stdout, FBLU("THREAD %d; sleep %d\n"), args.num, args.num);
	sleep(args.num);
		srand(time(NULL));
	glob_arr[args.num] = dRand(1, 10);
	fprintf(stdout, FGRN("THREAD %d; sleep %d\n"), args.num, glob_arr[args.num]);
	sleep(glob_arr[args.num]);
	return NULL;
}

void * func_detach(void * arg)
{
	while(1)
	{
		int cnt = 0;
		fprintf(stdout, FCYN("THREAD DETACHED:\n"));
		for(int i = 0; i < thread_count; i++)
		{
			if(glob_arr[i])
				cnt++;
			fprintf(stdout, FCYN("%d "), glob_arr[i]);
		}
		puts("");
		if(cnt == thread_count) break;
		sleep(1);
	}
	return NULL;
}

args_t * gen_args(int n)
{
	args_t * res = malloc(sizeof(args_t));
	res->num = n;
	return res;
}

int dRand(int dMin, int dMax)
{
	srand(time(NULL));
	return (rand() % (dMax + 1 - dMin)) + dMin;
}

void parsearg(int argc, char ** argv)
{
	int res, ind;
	const char * short_opt = "hn:";
	const struct option long_opt[] = {
		{"help", no_argument, NULL, 'h'},
		{"num",required_argument, NULL, 'n'},
		{NULL, 0, NULL, 0}
	};	
	while((res = getopt_long(argc, argv, short_opt, long_opt, &ind)) != -1)
	{
		switch(res)
		{
			case 'h':
				fprintf(stdout, "-n | --num [int]\tthread_count\n");
				break;
			case 'n':
				thread_count = atoi(optarg);
				break;
			case '?':
				break;
			default:
				break;
		}
	}
}
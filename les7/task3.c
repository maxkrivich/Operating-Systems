#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h> 
#include <getopt.h>
#include <stdlib.h>
#include <pthread.h>

#define handle_error_en(en, msg) \
		do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

typedef struct {
	int count;
}args_t;

int N = 0;

void parsearg(int, char **);
void * func(void *);

int main(int argc, char ** argv)
{
	parsearg(argc, argv);
	int s;
	pthread_t thread;
	args_t arg = {2 * N};

	s = pthread_create(&thread, NULL, &func, &arg);
	if(s) handle_error_en(s, "pthread_create");	
	
	sleep(N);
	
	s = pthread_cancel(thread);
	if(s) handle_error_en(s, "pthread_cancel");
	
	s = pthread_join(thread, NULL);
	if(s) handle_error_en(s, "pthread_join");
	
	return EXIT_SUCCESS;
}

void * func(void * arg)
{
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
	args_t * args = (args_t*)arg;
	for(int i = 0; i < args->count; i++)
	{
		fprintf(stdout, "Thread Child: %d\n", i);
		sleep(1);
	}
	return NULL;
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
				fprintf(stdout, "-n | --num [int]\tsleep sec\n");
				break;
			case 'n':
				N = atoi(optarg);
				break;
			case '?':
				break;
			default:
				break;
		}
	}
}
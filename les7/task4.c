#include <time.h>
#include <math.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h> 
#include <getopt.h>
#include <stdlib.h>
#include <pthread.h>

#define handle_error_en(en, msg) \
		do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

int N = 0;

void parsearg(int, char **);
void * func(void *);

int main(int argc, char ** argv)
{
	parsearg(argc, argv);
	pthread_t thread;
	int s;
	void * res;
	s = pthread_create(&thread, NULL, &func, NULL);
	if(s) handle_error_en(s, "main(): pthread_create");
	sleep(N);	
	s = pthread_cancel(thread);
	// if(s) handle_error_en(s, "main(): pthread_cancel");
	s = pthread_join(thread, &res);
	if (s) handle_error_en(s, "main(): pthread_join");
	if (res == PTHREAD_CANCELED)
		printf("main(): thread was canceled\n");
	else
		printf("main(): thread wasn't canceled\n");
	return EXIT_SUCCESS;
}

void * func(void * arg)
{
	int i = 1, n = 100000, s, old_cancel_state;
	long double pi, a, b;
	a = b = pi = 4.0;
	for(;i <= n; i++)
	{
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old_cancel_state);
			pi += a = (b = -b) / (2 * i + 1.0);   
		pthread_setcancelstate(old_cancel_state, NULL);
		fprintf(stdout, "pi = %.10Lf\n", pi);
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
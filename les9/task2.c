#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h> 
#include <getopt.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "colors.h"

#define handle_error_en(en, msg) \
		do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

typedef unsigned long ul;

/*-------------------VARs------------------------*/
int thread_count = 8, period = 2, sleeping_time = 3;
int glob_var;
pthread_t *threads = NULL;
pthread_cond_t cond;
pthread_mutex_t mutex;
/*-----------------------------------------------*/

/*-------------------FUNCs-----------------------*/
void parsearg(int, char **);
int dRand(int, int);
ul mix(ul, ul, ul);
void init(void);
void destroy(void);;
void * thread_producer(void *);
void * therad_consumer(void *);
/*-----------------------------------------------*/

int main(int argc, char ** argv)
{
	int s;
	parsearg(argc, argv);
	init();
	for(int i = 0; i < thread_count; i++)
	{
		if((i & 1) || (i < 2))
			s = pthread_create(&threads[i], NULL, therad_consumer, NULL);
		else
			s = pthread_create(&threads[i], NULL, thread_producer, NULL);

		if(s) handle_error_en(s, "main(): pthread_create");
	}

	sleep(sleeping_time);

	for(int i = 0; i < thread_count; i++)
	{
		s = pthread_cancel(threads[i]);
		if(s) handle_error_en(s, "main(): pthread_cancel");
	}

	return EXIT_SUCCESS;
}

/*-------------------FUNC IMPL--------------------*/
void init(void)
{
	atexit(destroy);
	int s;

	s = pthread_cond_init(&cond, NULL);
	if(s) handle_error_en(s, "init(): pthread_cond_init");

	s = pthread_mutex_init(&mutex, NULL);
	if(s) handle_error_en(s, "init(): pthread_mutex_init");

	threads = (pthread_t*) malloc(sizeof(pthread_t) * thread_count);

	if(threads == NULL)
		exit(EXIT_FAILURE);
}

void destroy(void)
{
	free(threads);
	pthread_cond_destroy(&cond);
	pthread_mutex_destroy(&mutex);
}

void * thread_producer(void * arg)
{
	int old_cancel_state;
	while(1)
	{
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old_cancel_state);
		pthread_mutex_lock(&mutex);
		glob_var = dRand(1, 100);
		fprintf(stdout, FCYN("id: %lu write: %d\n"), pthread_self(), glob_var);
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old_cancel_state);
		sleep(period);
	}
	return NULL;
}

void * therad_consumer(void * arg)
{
	int old_cancel_state;
	while(1)
	{
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old_cancel_state);
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond, &mutex);
		fprintf(stdout, FRED("id: %lu read: %d\n"), pthread_self(), glob_var);
		pthread_mutex_unlock(&mutex);
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old_cancel_state);
	}
	return NULL;
}

ul mix(ul a, ul b, ul c)
{
    a=a-b;  a=a-c;  a=a^(c >> 13);
    b=b-c;  b=b-a;  b=b^(a << 8);
    c=c-a;  c=c-b;  c=c^(b >> 13);
    a=a-b;  a=a-c;  a=a^(c >> 12);
    b=b-c;  b=b-a;  b=b^(a << 16);
    c=c-a;  c=c-b;  c=c^(b >> 5);
    a=a-b;  a=a-c;  a=a^(c >> 3);
    b=b-c;  b=b-a;  b=b^(a << 10);
    c=c-a;  c=c-b;  c=c^(b >> 15);
    return c;
}

int dRand(int dMin, int dMax)
{
	srand(mix(clock(), time(NULL), pthread_self()));
	return (rand() % (dMax + 1 - dMin)) + dMin;
}

void parsearg(int argc, char ** argv)
{
	int res, ind;
	const char * short_opt = "ht:p:s:";
	const struct option long_opt[] = {
		{"help", no_argument, NULL, 'h'},
		{"thread",required_argument, NULL, 't'},
		{"period", required_argument, NULL, 'p'},
		{"sleep", required_argument, NULL, 's'},
		{NULL, 0, NULL, 0}
	};	
	while((res = getopt_long(argc, argv, short_opt, long_opt, &ind)) != -1)
	{
		switch(res)
		{
			case 'h':
				fprintf(stdout, "-t | --therad [int]\tthread_count\n");
				break;
			case 't':
				thread_count = atoi(optarg);
				break;
			case 'p':
				period = atoi(optarg);
				break;
			case 's':
				sleeping_time = atoi(optarg);
				break;
			case '?':
				break;
			default:
				break;
		}
	}
}
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

#define size 10

typedef unsigned long ul;

/*-------------------VARs------------------------*/
// const int size = 10;
int thread_count = 8, sleeping_time = 1;
int ptr;
sem_t lock, empty_items, full_items;
int buff[size];
pthread_t *threads = NULL;
/*-----------------------------------------------*/

/*-------------------FUNCs-----------------------*/
void parsearg(int, char **);
int dRand(int, int);
ul mix(ul, ul, ul);
void init(void);
void destroy(void);
void add_to_buff(int);
int get_last(void);
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
			s = pthread_create(&threads[i], NULL, thread_producer, NULL);
		else
			s = pthread_create(&threads[i], NULL, therad_consumer, NULL);

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
	s = sem_init(&lock, 0, 1);
	if(s) handle_error_en(s, "init(): sem_init");

	s = sem_init(&empty_items, 0, size);
	if(s) handle_error_en(s, "init(): sem_init");	

	s = sem_init(&full_items, 0, 0);
	if(s) handle_error_en(s, "init(): sem_init");

	threads = (pthread_t*) malloc(sizeof(pthread_t) * thread_count);

	if(threads == NULL)
		exit(EXIT_FAILURE);
}

void destroy(void)
{
	free(threads);
	sem_destroy(&lock);
	sem_destroy(&empty_items);
	sem_destroy(&full_items);
}

void * thread_producer(void * arg)
{
	int old_cancel_state;
	while(1)
	{
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old_cancel_state);
		int r = dRand(1, 100);
		sem_wait(&empty_items);
		sem_wait(&lock);
		add_to_buff(r);
		sem_post(&lock);
		sem_post(&full_items);
		fprintf(stdout, FBLU("id: %lu\twrite: %d\tsize: %d\n"), pthread_self(), r, ptr);
		pthread_setcancelstate(old_cancel_state, NULL);
	}
	return NULL;
}

void * therad_consumer(void * arg)
{
	int old_cancel_state;
	while(1)
	{
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old_cancel_state);
		int r;
		sem_wait(&full_items);
		sem_wait(&lock);
		r = get_last();
		sem_post(&lock);
		sem_post(&empty_items);
		fprintf(stdout, FYEL("id: %lu\tread: %d\tsize: %d\n"), pthread_self(), r, ptr);
		pthread_setcancelstate(old_cancel_state, NULL);
	}
	return NULL;
}

void add_to_buff(int rand)
{
	sem_getvalue(&full_items, &ptr);
	buff[ptr] = rand;
}

int get_last(void)
{
	sem_getvalue(&full_items, &ptr);
	return buff[ptr];
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
	const char * short_opt = "ht:s:";
	const struct option long_opt[] = {
		{"help", no_argument, NULL, 'h'},
		{"thread",required_argument, NULL, 't'},
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
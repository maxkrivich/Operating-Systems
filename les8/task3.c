#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h> 
#include <getopt.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include "colors.h"

#define handle_error_en(en, msg) \
		do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

typedef unsigned long ul;

int N = 10, thread_count = 5;
int *arr = NULL;
pthread_rwlock_t lock;

void parsearg(int, char **);
void * thread_write(void *);
void * thread_read(void *);
long dRand(long, long);
ul mix(ul, ul, ul);

int main(int argc, char ** argv)
{
	int s;
	parsearg(argc, argv);
	arr = (int *) malloc(N * sizeof(int));
	pthread_t *th_arr =(pthread_t *) malloc(thread_count * sizeof(pthread_t));
	s = pthread_rwlock_init(&lock, NULL);
	if(s) handle_error_en(s, "main(): pthread_rwlock_init");
	for(int i = 0; i < thread_count; i++)
	{
		if((i & 1) || (i < 1))
			s = pthread_create(&th_arr[i], NULL, thread_write, NULL);
		else 
			s = pthread_create(&th_arr[i], NULL, thread_read, NULL);
		if(s) handle_error_en(s, "main(): pthread_create");
	}

	for(int i = 0; i < thread_count; i++)
	{
		s = pthread_join(th_arr[i], NULL);
		if(s) handle_error_en(s, "main(): pthread_join");
	}

	free(arr);
	free(th_arr);
	pthread_rwlock_destroy(&lock);
	return EXIT_SUCCESS;
}


void * thread_write(void * arg)
{
	while(1)
	{
		int r = dRand(INT_MIN >> 4, INT_MAX), pos = dRand(0, N);
		pthread_rwlock_wrlock(&lock);
		arr[pos] = r;
		fprintf(stdout, FRED("write: arr[%d] = %d\n"), pos, r);
		pthread_rwlock_unlock(&lock);
		sleep(dRand(1, 3));
	}
	return NULL;
}


void * thread_read(void * arg)
{
	while(1)
	{
		int pos = dRand(0, N);
		pthread_rwlock_rdlock(&lock);
		fprintf(stdout, FCYN("read: arr[%d] = %d\n"), pos, arr[pos]);
		pthread_rwlock_unlock(&lock);
		sleep(dRand(1, 3));
	}
	return NULL;
}


void parsearg(int argc, char ** argv)
{
	int res, ind;
	const char * short_opt = "hp:n:";
	const struct option long_opt[] = {
		{"help", no_argument, NULL, 'h'},
		{"pthread",required_argument, NULL, 'p'},
		{"num", required_argument, NULL, 'n'},
		{NULL, 0, NULL, 0}
	};	
	while((res = getopt_long(argc, argv, short_opt, long_opt, &ind)) != -1)
	{
		switch(res)
		{
			case 'h':
				fprintf(stdout, "-p | --ptherad [int]\tthread_count\n-n | --num [int]\tarray size\n");
				exit(-1);
				break;
			case 'p':
				thread_count = atoi(optarg);
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

long dRand(long dMin, long dMax)
{
	srand(mix(clock(), time(NULL), pthread_self()));
	return (rand() % (dMax + 1 - dMin)) + dMin;
}
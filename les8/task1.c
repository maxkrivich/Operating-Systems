#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h> 
#include <getopt.h>
#include <stdlib.h>
#include <pthread.h>
#include "colors.h"

#define handle_error_en(en, msg) \
		do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

typedef unsigned long ul;

int thread_count = 10;
int edit_count = 2;
int glob_var = 0;

void parsearg(int, char **);
void * inc(void *);
void * dec(void *);
ul mix(ul, ul, ul);
int dRand(int, int);

int main(int argc, char ** argv)
{
	parsearg(argc, argv);
	int s;
	pthread_t *arr = malloc(thread_count * sizeof(pthread_t));
	for(int i = 0; i < thread_count; i++)
	{
		if(i & 1)	s = pthread_create(&arr[i], NULL, inc, NULL);
		else	s = pthread_create(&arr[i], NULL, dec, NULL);
		if(s) handle_error_en(s, "main(): pthread_create");
	}
	for(int i = 0; i < thread_count; i++)
	{
		s = pthread_join(arr[i], NULL);
		if(s) handle_error_en(s, "main(): pthread_join");
	}
	fprintf(stdout, BOLD("VAR: %d\n"), glob_var);
	return EXIT_SUCCESS;
}

void * inc(void * arg)
{
	for(int i = 0; i < edit_count; i++)
	{
		fprintf(stdout, FYEL("ID: %lu VAR: %d\n"), pthread_self(), ++glob_var);
		sleep(dRand(1, 10));
	}
	return NULL;
}

void * dec(void * arg)
{
	for(int i = 0; i < edit_count; i++)
	{
		fprintf(stdout, FBLU("ID: %lu VAR: %d\n"), pthread_self(), --glob_var);
		sleep(dRand(1, 10));
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
	srand(mix(clock(), time(NULL), getpid()));
	return (rand() % (dMax + 1 - dMin)) + dMin;
}

void parsearg(int argc, char ** argv)
{
	int res, ind;
	const char * short_opt = "ht:e:";
	const struct option long_opt[] = {
		{"help", no_argument, NULL, 'h'},
		{"thread",required_argument, NULL, 't'},
		{"edit", required_argument, NULL, 'e'},
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
			case 'e':
				edit_count = atoi(optarg);
				break;
			case '?':
				break;
			default:
				break;
		}
	}
}
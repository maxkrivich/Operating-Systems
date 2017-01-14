#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h> 
#include <getopt.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include "colors.h"

#define handle_error_en(en, msg) \
		do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

int thread_count = 0;
static pthread_key_t key;
static pthread_once_t once = PTHREAD_ONCE_INIT; 

typedef struct {
	int a;
	int b;
	char * str;
} args_t;

typedef unsigned long ul;

static void once_creator(void)
{
	pthread_key_create(&key, NULL);
}

void parsearg(int, char **);
void * func(void *);
int dRand(int, int);
char * get_rand_str(size_t);
ul mix(ul, ul, ul);
void print(int);
args_t * gen_arg(int, int, char *);


int main(int argc, char ** argv)
{
	parsearg(argc, argv);
	int s;
	pthread_t * arr = malloc(thread_count * sizeof(pthread_t));
	for(int i = 0; i < thread_count; i++)
	{
		s = pthread_create(&arr[i], NULL, &func, gen_arg(1, 10, get_rand_str(dRand(1, 20))));
		if(s) handle_error_en(s, "main(): pthread_create");
	}
	for(int i = 0; i < thread_count; i++)
	{
		s = pthread_join(arr[i], NULL);
		if(s) handle_error_en(s, "main(): pthread_join");
	}
	return 0;
}

void print(int rand)
{
	char *str = (char *) pthread_getspecific(key);
	fprintf(stdout, "%s RAND: %d\n", str, rand);
}

void * func(void * arg)
{
	args_t * args = (args_t*)arg;
	char str[256];
	pthread_once(&once, once_creator);
	sprintf(str, "ID: %lu STR: %s ", pthread_self(), args->str);
	pthread_setspecific(key, str);
	int cnt = dRand(args->a, args->b);
	while(cnt--)
		print(dRand(args->a, args->b));
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
	// struct timeval time; 
	// gettimeofday(&time,NULL);
	// srand((time.tv_sec * 1000) + (time.tv_usec / 1000));
	srand(mix(clock(), time(NULL), getpid()));
	return (rand() % (dMax + 1 - dMin)) + dMin;
}

char * get_rand_str(size_t len)
{
	char * str = malloc(sizeof(char) * len + 1);
	char * al = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
	int al_le = strlen(al);
	if(len)
	{
		str[len] = '\0';
		for(int i = 0; i < len; i++)
			str[i] = al[dRand(0, al_le - 1)];
	}
	return str;
}

args_t * gen_arg(int a, int b, char * str)
{
	args_t * res = malloc(sizeof(args_t));
	res->a = a;
	res->b = b;
	res->str = str;
	return res;
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
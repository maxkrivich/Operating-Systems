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

double res = .0, a = .0, b = 2 * atan(1.0), h;
int n = 100000, loc_n;
pthread_mutex_t mut;
int thread_count;

void parsearg(int, char **);
double f(double);
void * func(void *);
double integate(double, double);

int main(int argc, char ** argv)
{
	parsearg(argc, argv);
	int s;
	h = (b - a) / n;
	loc_n = n / thread_count;
	pthread_t * arr = malloc(thread_count * sizeof(pthread_t));
	s = pthread_mutex_init(&mut, NULL);
	if(s) handle_error_en(s, "main(): pthread_mutex_init");
	for(int i = 0; i < thread_count; i++)
	{
		s = pthread_create(&arr[i], NULL, func, (void*) i);
		if(s) handle_error_en(s, "main(): pthread_create");
	}

	for(int i = 0; i < thread_count; i++)
	{
		s = pthread_join(arr[i], NULL);
		if(s) handle_error_en(s, "main(): pthread_join");
	}

	fprintf(stdout, "INTEGRAL: %g\n", res);
	pthread_mutex_destroy(&mut);
	free(arr);
	
	return EXIT_SUCCESS;
}

double f(double x)
{
	return cos(x);
}

void * func(void * arg)
{
	double loc_a, loc_b, ans;
	int my_rank = (int) arg;
	loc_a = a + my_rank * loc_n * h;
	loc_b = loc_a + loc_n * h;
	ans = integate(loc_a, loc_b);
	pthread_mutex_lock(&mut);
	res += ans;
	pthread_mutex_unlock(&mut);
	fprintf(stdout, "%lu:\t%g\n", pthread_self(), ans);
	return NULL;
}

double integate(double loc_a, double loc_b)
{
	double integ = (f(loc_a) + f(loc_b)) * 0.5, x = loc_a;
	for (int i = 1; i <= loc_n - 1; ++i)
	{
		x = loc_a + i * h;
		integ += f(x);
	}	
	integ = integ * h;
	return integ;
}

void parsearg(int argc, char ** argv)
{
	int res, ind;
	const char * short_opt = "hp:";
	const struct option long_opt[] = {
		{"help", no_argument, NULL, 'h'},
		{"pthread",required_argument, NULL, 'p'},
		{NULL, 0, NULL, 0}
	};	
	while((res = getopt_long(argc, argv, short_opt, long_opt, &ind)) != -1)
	{
		switch(res)
		{
			case 'h':
				fprintf(stdout, "-p | --ptherad [int]\tthread_count\n");
				exit(-1);
				break;
			case 'p':
				thread_count = atoi(optarg);
				break;
			case '?':
				break;
			default:
				break;
		}
	}
}
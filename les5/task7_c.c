#include <stdio.h>
#include <stdlib.h>
#include <getopt.h> 
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define sq(a) (a) * (a)

int n, a, b, c, d, sp, sm, m = 0, g = 10;
double x, y;

void parsearg(int, char *[]);
int in_m(double x, double y);
double fRand(double , double);

int main(int argc, char * argv[])
{
	parsearg(argc, argv);
	a = 0;
	b = a + g;
	c = 0;
	d = c + g;
	struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    srand((time_t)ts.tv_nsec);
	for(int i = 0; i < n; i++)
	{
		x = fRand(a, b);
		y = fRand(c, d);
		// printf("PID:%d x = %f y = %f\n", getpid(), x, y);
		if(in_m(x, y))
			m++;
	}
	printf("PID:%d %d\n",getpid(), m);
	return m;
}

double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

int in_m(double x, double y)
{
	double  xc = a + (double)b/2,
			yc = c + (double)d/2,
			r = (d-c)/2;
	return sq((x-xc)) + sq((y-yc)) <= sq(r);
}

void parsearg(int argc, char * argv[])
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
				exit(EXIT_SUCCESS);
				break;
			case 'n':
				// printf("HEre\n");
				n = atoi(optarg);
				break;
			case '?':
				break;
			default:
				break;
		}
	}
}
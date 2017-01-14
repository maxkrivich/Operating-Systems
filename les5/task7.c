#include <wait.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <math.h>

int main(int argc, char ** argv)
{
	char * arg[] = {argv[0], "-n 255", NULL};
	pid_t chpid, sts;
	int es, ans, ch_cnt = 10, n = 0, m = 0;

	for(int i = 0; i < ch_cnt; i++, n += 255)
	{
		if(fork() == 0)
		{
			execve("task7_c.out", arg, NULL);
			exit(EXIT_FAILURE);
		}
	}
	while(wait(&es) > 0)
		m += WEXITSTATUS(es);
	printf("m = %d n = %d\n", m, n);
	double pi = (double) m*4/n;
	printf("PI = %.5g\n", pi);
	return 0;
}
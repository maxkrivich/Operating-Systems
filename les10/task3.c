#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <getopt.h>
#include <sys/wait.h>
#include <sys/types.h>

#define mins(a) (a) * 3

typedef unsigned int uint;

/*-------------------VARs------------------------*/
uint time = mins(1);
char * str = "Hello";
/*-----------------------------------------------*/

/*-------------------FUNCs-----------------------*/
void parsearg(int, char **);
void sg_handler(int);
/*-----------------------------------------------*/

int main(int argc, char ** argv)
{
	parsearg(argc, argv);

	if(signal(SIGALRM, sg_handler) == SIG_ERR)
	{
		fprintf(stderr, "\ncan't catch SIGALRM\n");
		exit(EXIT_FAILURE);
	}

	pid_t pid = fork();


	if(!pid)
	{
		alarm(time);
		pause();
		fprintf(stdout, "%s\n", str);
	}
	else
		return EXIT_SUCCESS;

	// if(!pid)
	// {
	// 	pause();
	// 	fprintf(stdout, "%s\n", str);
	// 	exit(EXIT_SUCCESS);
	// }
 // 	else if (pid > 0)
 // 	{
 // 		alarm(time);
	// 	pause();
	// 	kill(pid, SIGALRM);
	// 	exit(EXIT_SUCCESS);
	// }

	return EXIT_FAILURE;
}

/*-------------------FUNC IMPL--------------------*/

void parsearg(int argc, char ** argv)
{
	int res, ind;
	const char * short_opt = "ht:s:";
	const struct option long_opt[] = {
		{"help", no_argument, NULL, 'h'},
		{"time", required_argument, NULL, 't'},
		{"string", required_argument, NULL, 's'},
		{NULL, 0, NULL, 0}
	};
	while((res = getopt_long(argc, argv, short_opt, long_opt, &ind)) != -1)
	{
		switch(res)
		{
			case 'h':
				printf("-t | --time [int] - seep time\n-s | --string [string] - string\n");
				exit(EXIT_FAILURE);
				break;
			case 't':
				time = mins(atoi(optarg));
				break;
			case 's':
				str = optarg;
			case '?':
				break;
			default:
				break;
		}
	}
}

void sg_handler(int signo)
{
}
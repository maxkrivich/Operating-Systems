#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>

extern const char * const sys_siglist[];

int sleep_time = 1;

void sg_handler(int signo)
{
	if(signo != SIGALRM)
	{
		alarm(0);
	}
}

int main(void)
{
	if(signal(SIGUSR1, sg_handler) == SIG_ERR)
	{
		fprintf(stderr, "\ncan't catch SIGUSR1\n");
		exit(EXIT_FAILURE);
	}

	if(signal(SIGALRM, sg_handler) == SIG_ERR)
	{
		fprintf(stderr, "\ncan't catch SIGUSR1\n");
		exit(EXIT_FAILURE);
	}

	pid_t pid = fork();

	switch(pid)
	{
		case -1:
			exit(EXIT_FAILURE);
		case 0:
			while(1)
			{
				alarm(sleep_time);
				pause();
				fprintf(stdout, "child process pid %d\n", getpid());
				kill(getppid(), SIGUSR1);
				
			}
			break;
		default:
			while(1)
			{
				fprintf(stdout, "parent process pid %d\n", getpid());
				kill(pid, SIGUSR1);
				alarm(sleep_time);
				pause();
			}
			break;
	}
	return EXIT_FAILURE;
}

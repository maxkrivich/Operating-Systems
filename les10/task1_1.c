#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>

void sig_handler(int signo)
{
	switch(signo)
	{
		case SIGINT:
			fprintf(stdout, "\nreceived SIGINT\n");
			break;
		case SIGTERM:
			fprintf(stdout, "\nreceived SIGTERM\n");
			break;
	}
}

int main(void)
{
	printf("%d\n", getpid());
	if(signal(SIGINT, sig_handler) == SIG_ERR) // ctrl + C
	{ 
		fprintf(stderr, "\ncan't catch SIGINT\n");
		exit(EXIT_FAILURE);
	}

	if(signal(SIGTERM, sig_handler) == SIG_ERR)
	{
		fprintf(stderr, "\ncan't catch SIGTERM\n");
		exit(EXIT_FAILURE);
	}

	if(signal(SIGPROF, SIG_DFL) == SIG_ERR)
	{
		fprintf(stderr, "\ncan't catch SIGPROF\n");
		exit(EXIT_FAILURE);
	}

	if(signal(SIGHUP, SIG_IGN) == SIG_ERR)
	{
		fprintf(stderr, "\ncan't catch SIGHUP\n");
		exit(EXIT_FAILURE);
	}

	while(1)
		pause();

	return 0;
}
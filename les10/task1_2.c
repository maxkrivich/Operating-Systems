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
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask, SIGPROF);
	sigaddset(&mask, SIGHUP);
	sigprocmask(SIG_SETMASK, &mask, NULL);
	struct sigaction new_sa;
	new_sa.sa_handler = sig_handler;
	new_sa.sa_mask = mask;
	new_sa.sa_flags = 0;

	if(sigaction(SIGINT, &new_sa, NULL))
	{
		fprintf(stderr, "\ncan't catch SIGINT\n");
		exit(EXIT_FAILURE);
	}

	if(sigaction(SIGTERM, &new_sa, NULL))
	{
		fprintf(stderr, "\ncan't catch SIGTERM\n");
		exit(EXIT_FAILURE);
	}

	while(1)
		pause();

	return 0;
}
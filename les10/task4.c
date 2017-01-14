#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#define SIGNAL_COUNT 10

extern const char * const sys_siglist[];

void sighand(int signo, siginfo_t *info, void *extra)
{
	if(signo == SIGTERM)
	{
		exit(EXIT_SUCCESS);
	}
	else
		fprintf(stdout, "PID: %d, signal: %s, value: [%d]\n", getpid(), sys_siglist[signo], info->si_value.sival_int);
}

int main(void)
{	
	// sigset_t mask;
	// sigset_t old_mask;

	// sigemptyset(&mask);
	// sigaddset(&mask, SIGUSR1);

	// sigprocmask(SIG_BLOCK, &mask, &old_mask);

	struct sigaction action;
	// sigemptyset(&action.sa_mask); 
	action.sa_flags = SA_SIGINFO;
	action.sa_sigaction = sighand;

	sigaction(SIGUSR1, &action, NULL);
	sigaction(SIGTERM, &action, NULL);

	pid_t child_pid = fork();

	if(child_pid == 0)
	{
		while(1)
		{
			pause();
			// sigsuspend(&old_mask);
		}
	}
	else
	{
		for(int i = 0; i < SIGNAL_COUNT; i++)
		{
			union sigval v;
			v.sival_int = i;
			int res = sigqueue(child_pid, SIGUSR1, v);
			strerror(errno);
			// sleep(0);
		}
		int status;
		sigqueue(child_pid, SIGTERM, (const union sigval) NULL); // EXIT SIGNAL
		wait(&status);
		fprintf(stdout, "Parent PID: %d, Child PID: %d\n", getpid(), child_pid);
	}
	// sigprocmask(SIG_SETMASK, &old_mask, NULL);
	
	return EXIT_SUCCESS;
}
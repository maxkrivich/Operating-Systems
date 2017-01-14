#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <errno.h>


int main(int argc, char ** argv)
{
	extern char ** environ;
	char * args[] = {"ls", "-l", NULL};
	int sts;
	pid_t chp;
	if(!fork())
	{
		execve("/bin/ls", args, environ);
		fprintf (stderr, "Execve Error\n");
		exit(EXIT_FAILURE);
	}
	chp = wait(&sts);

	if(-1 == chp)
	{
		fprintf(stderr, "Wait Error\n");
		exit(EXIT_FAILURE);
	}

	if((WIFEXITED(sts)))
		return EXIT_SUCCESS;
	else
		return EXIT_FAILURE;
}
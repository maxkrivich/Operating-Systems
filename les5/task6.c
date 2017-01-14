#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>

int main (void)
{
	int status;
	/* Child */
	if (!fork())
	{
		execlp("ls", "ls", NULL);
		fprintf (stderr, "Exec error\n");
	}
	/* Parent */
	sleep(10);
	system("ps aux | grep 'Z'");
	sleep(30);
	wait(&status);
	if (WIFEXITED (status)) 
	{
		printf ("Code=%d\n", WEXITSTATUS(status));
	}
	return 0;
}
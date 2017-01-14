#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>

#define PRINT(var) fprintf(stdout, "parentID = %d; ID = %d; var = %d; addr = %p\n", getppid(), getpid(), (var), &(var));

int main(int argc, char ** argv)
{
	int a = 0;
	pid_t p = fork();
	a++;
	PRINT(a);
	if(p)
	{
		fork();
		a++;
		PRINT(a);
	}
	else if (!p)
	{
		fork();
		a++;
		PRINT(a);
	}
	return 0;
}
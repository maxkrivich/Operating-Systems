#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

int main(void) 
{
	pid_t pid;
	unsigned int eps = 4;
	fprintf(stdout, "Пока всего один процесс\n");
	fprintf(stdout, "Вызов функции fork...\n");
	pid = fork();
	if (pid == 0)
	{
		fprintf(stdout, "Процесс - потомок\n");
		int cnt = 0;
		time_t t = time(NULL);
		while (time(NULL) - t < eps)
			++cnt;
		fprintf(stdout,"Процесс - потомок, cnt = %d\n", cnt);
	} else if (pid > 0) 
	{
		fprintf(stdout, "Родительский процесс, pid потомка %d\n", pid);
		int cnt = 0;
		time_t t = time(NULL);
		while (time(NULL) - t < eps)
			++cnt;
		fprintf(stdout,"Родительский процесс, cnt = %d\n", cnt);
	} else 
	{
		fprintf(stderr,"Ошибка вызова fork, потомок не создан\n"); 
	}
	return 0;
}
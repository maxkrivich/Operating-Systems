#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>

char * path;

int my_execvp(const char * name, char * argv[])
{
	assert(name);
	assert(name[0] != '\0');
	assert(path);
	int len1 = strlen(name), len2;
	char buf[256];
	if (strchr(name, '/')) goto try_vp;
	for (char * p = strtok(path, ":"); p; p = strtok(NULL, ":"))
	{
		len2 = strlen(p);
		memcpy(buf,p, len2);
		buf[len2] = '/';
		memcpy(buf+len2+1,name,len1);
		buf[len2+len1+1] = '\0';
		#ifdef DEBUG
			printf("path = %s\n", buf);
		#endif
try_vp: 	execv(buf, argv);
		switch(errno)
		{
			case EACCES:
			case ENOTDIR:
			case ENOENT:
			case ENOEXEC:
			case ETXTBSY:
			default:
				break;
		}
	}
	return -1;
}

int  my_execlp(const char * name, const char * arg, ...)
{
	va_list va;
	char buf[256];
	int len1 = strlen(name), len2;
	va_start(va, arg);
	if(strchr(name, '/')) goto try_lp;
	for (char * p = strtok(path, ":"); p; p = strtok(NULL, ":"))
	{
		len2 = strlen(p);
		memcpy(buf,p, len2);
		buf[len2] = '/';
		memcpy(buf+len2+1,name,len1);
		buf[len2+len1+1] = '\0';
		#ifdef DEBUG
			printf("path = %s\n", buf);
		#endif
try_lp:		execl(buf, arg, NULL); //это конец!!
	}
	va_end(va);
	return -1;
}


int main(int argc, char ** argv)
{
	path = getenv("PATH");
	char * arg[] = {"ls","-l", NULL};
	switch(atoi(argv[1]))
	{
		case 1:
			my_execvp("ls", arg);
			break;
		case 2:
			my_execlp("ls", "ls", "-l", NULL);
			break;
	}
	return EXIT_SUCCESS;
}
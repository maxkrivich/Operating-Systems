#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#define BUFF 256

char* str;
char* rstr;

void handler(int signo) {
	if(signo == SIGUSR2)
		exit(EXIT_SUCCESS);
}

void init(void){
	rstr = malloc(BUFF * sizeof(char));
	str = malloc(BUFF * sizeof(char));

}

void destpoy(void){
	free(rstr);
	free(str);
}

int main(void) {

	atexit(destpoy);
	init();
	int fd[2]; //fd[0] - read fd[1] - write
	pid_t pid;
	size_t size, len;

	if(signal(SIGUSR1, handler) == SIG_ERR) {
		fprintf(stderr, "\ncan't catch SIGALRM\n");
		exit(-1);
	}
	if(signal(SIGUSR2, handler) == SIG_ERR) {
		fprintf(stderr, "\ncan't catch SIGALRM\n");
		exit(-1);
	}

	if(pipe(fd) < 0) {
		fprintf(stderr, "Can\'t create pipe\n");
		exit(-1);
	}

	pid = fork();

	if(pid) {

		while(1) {
			fprintf(stdout, "INPUT STRING: ");
			fgets(str, BUFF, stdin);
			if(!strcmp(str,"stop\n")) {
				kill(pid,SIGUSR2);
				close(fd[1]);
				close(fd[0]);
				exit(EXIT_SUCCESS);
			}
			len = strlen(str);
			size = write(fd[1], str, BUFF);
			if(size != BUFF) {
				fprintf(stdout, "Can\'t write all string\n");
				exit(-1);
			}
			kill(pid, SIGUSR1);
			pause();
			size = read(fd[0], str, BUFF);
			if(size != BUFF) {
				fprintf(stdout, "Can\'t read all string\n");
				exit(-1);
			}
			fprintf(stdout, "RETURNED STRING: ");
			puts(str);
		}
	}else if(!pid) {
		while(1) {
			pause();
			size = read(fd[0], rstr, BUFF);
			if(size != BUFF) {
				fprintf(stdout, "Can\'t read all string\n");
				exit(-1);
			}
			len = strlen(rstr);
			for(int i = 0; i < len; i++)
				if(isalpha(rstr[i]))
					rstr[i] = toupper(rstr[i]);
			size = write(fd[1], rstr, BUFF);
			if(size != BUFF) {
				fprintf(stdout, "Can\'t write all string\n");
				exit(-1);
			}
			kill(getppid(), SIGUSR1);
		}
	} else {
		fprintf(stderr, "Can\'t create process\n");
		exit(-1);
	}
	return EXIT_SUCCESS;
}
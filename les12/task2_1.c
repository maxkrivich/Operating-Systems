#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BUFF 256
#define NAME1 "aaa.fifo"
#define NAME2 "bbb.fifo"


int main(int argc, char *argv[]) {
	int w, r;
	char* str = malloc(BUFF * sizeof(char));

	w = open(NAME1, O_WRONLY /*| O_NONBLOCK*/);
	r = open(NAME2, O_RDONLY /*| O_NONBLOCK*/);

	while(1) {
		fprintf(stdout, "INPUT STRING: ");
		fgets(str, BUFF, stdin);
		write(w, str, BUFF);
		if(!strcmp(str, "stop\n")) break;
		read(r, str, BUFF);
		printf("RETUENED STRING: %s\n", str);
	}

	free(str);
	close(w);
	close(r);

	return EXIT_SUCCESS;
}
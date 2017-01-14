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

	mkfifo(NAME1, 0666);
	mkfifo(NAME2, 0666);
	// (void)umask(0);
	// mknod(NAME1, S_IFIFO | 0666, 0);
	// mknod(NAME2, S_IFIFO | 0666, 0);

	r = open(NAME1, O_RDONLY /*| O_NONBLOCK*/);
	w = open(NAME2, O_WRONLY /*| O_NONBLOCK*/);
	

	while(1) {
		read(r, str, BUFF);	
		if(!strcmp(str, "stop\n")) break;
		int len = strlen(str);
		for(int i = 0; i < len; i++)
			if(isalpha(str[i]))
				str[i] = toupper(str[i]);
		write(w, str, BUFF);
	}

	free(str);
	close(w);
	close(r);	

	return EXIT_SUCCESS;
}
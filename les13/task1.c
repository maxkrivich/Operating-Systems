#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
	int fd;
	char buf1[] = "abcdefghij";
	char buf2[] = "ABCDEFGHIJ";

	if ((fd = open("file.hole", O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0)
		printf("creat error");

	if (write(fd, buf1, 10) != 10)
		printf("buf1 write error");

	if (lseek(fd, 16384, SEEK_SET) == -1)
		printf("lseek error");

	if (write(fd, buf2, 10) != 10)
		printf("buf2 write error");

	return 0;
}
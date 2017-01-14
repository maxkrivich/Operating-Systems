#include <stdio.h>
#include <stdlib.h>
#include <getopt.h> 
#include <unistd.h>
#include <string.h>
#include "task4.h"
#include "colors.h"

size_t buff;
char * user_name;
char * command;

void init(void)
{
	user_name = getenv("USER");
	command = (char *) malloc(sizeof(char) * buff);
	atexit(wait3);
}

void parsearg(int argc, char ** argv)
{
	int res, ind;
	const char * short_opt = "hb:";
	const struct option long_opt[] = {
		{"help", no_argument, NULL, 'h'},
		{"buffer",required_argument, NULL, 'b'},
		{NULL, 0, NULL, 0}
	};
	buff = 127;
	while((res = getopt_long(argc, argv, short_opt, long_opt, &ind)) != -1)
	{
		switch(res)
		{
			case 'h':
				printf(FRED("-b | --buffer [int] - buffer size\n"));
				exit(EXIT_SUCCESS);
				break;
			case 'b':
				buff = atoi(optarg);
				break;
			case '?':
				break;
			default:
				break;
		}
	}
}

void wait3(void)
{
	fprintf(stderr, FBLU("Wait 3 seconds...\n"));
	sleep(3);
	free(command);
	system("clear");
}

int main(int argc, char ** argv)
{
	parsearg(argc, argv);
	init();
	system("clear");
	printf("Hello, " BOLD("%s!\n"), user_name);
	do
	{
		fprintf(stdout,FGRN("[%s]")FMAG("$ "), user_name);
		fscanf(stdin,"%[^\n]%*c", command);
		// fgets(command, buff, stdin);
		if (!strcmp(command, "stop"))
			break;
		int ret = system(command);
		if(ret != EXIT_SUCCESS)
		{
			fprintf(stderr, FYEL("command = %s \nexit code = %d\n"), command, ret);
			exit(EXIT_FAILURE);
		}
	}while(1);
	return EXIT_SUCCESS;
}
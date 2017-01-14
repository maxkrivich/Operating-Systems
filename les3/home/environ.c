#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include "option.h"

extern char ** environ;

void parseargs(int argc, char *argv[])
{
	if(argc == 1)
	{
		// system("env");
		for(int i = 0; environ[i]; i++)
			fprintf(stdout, "%s\n",environ[i]);
	} else
	{
		const char * short_options = "hi:s:d:ca:v:";
		const struct option long_option[] = {
			{"help", no_argument, NULL, 'h'},
			{"info", required_argument, NULL, 'i'},
			{"set", required_argument, NULL, 's'},
			{"assign", required_argument, NULL, 'a'},
			{"value", required_argument, NULL, 'v'},
			{"del", required_argument, NULL, 'd'},
			{"clear", no_argument, NULL, 'c'},
			{NULL, 0, NULL, 0}
		};
		int res;
		int option_index;
		while((res = getopt_long(argc, argv, short_options, long_option, &option_index)) != -1)
		{
			switch(res)
			{
				case 'a':
					seta(optarg);
					break;
				case 'v':
					setv(optarg);
					break;
				case 'h':
					help();
					break;
				case 'i':
					infovar(optarg);
					break;
				case 's':
					// char *pt = strdup(optarg);
					setvar(optarg);
					// free(pt);
					break;
				case 'd':
					deletevar(optarg);
					break;
				case 'c':
					clearenvr();
					break;
				case '?':
					break;
				default:
					break;
			}
		}
		avopt();
	}
}

int main(int argc, char *argv[])
{
	parseargs(argc, argv);	
	return 0;
}
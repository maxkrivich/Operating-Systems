#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>

#define SEMNUM 3

int BUFF_SIZE;
int shm_id;

void sg_handler(int);
char *append(const char *, char );
void sighand(int signo, siginfo_t *info, void *extra)
{
	shm_id = info->si_value.sival_int;
}

int main(int argc, char const *argv[])
{

	sigset_t mask;
	sigset_t old_mask;

	sigemptyset(&mask);
	sigaddset(&mask, SIGUSR1);

	sigprocmask(SIG_BLOCK, &mask, &old_mask);

	struct sigaction action;
	sigemptyset(&action.sa_mask); 
	action.sa_flags = SA_SIGINFO;
	action.sa_sigaction = sighand;

	sigaction(SIGUSR1, &action, NULL);


	if(signal(SIGUSR2, sg_handler) == SIG_ERR)
	{
		fprintf(stderr, "\ncan't catch SIGUSR1\n");
		exit(EXIT_FAILURE);
	}

	char *shm_buf;
	bool flag = false;

	if(argc < 3)
	{
		flag = true;
	}

	if(flag)
	{
		sleep(1);
		kill(getppid(), SIGUSR2);
		pause();
		BUFF_SIZE = atoi(argv[1]);

		kill(getppid(), SIGUSR2);
		// signal 
	}
	else
	{
		shm_id = atoi(argv[1]);
		BUFF_SIZE = atoi(argv[2]);
	}

	shm_buf = (char *) shmat(shm_id, NULL, 0);
	if (shm_buf == (char *) -1) 
	{
		fprintf(stderr, "shmat() error\n");
		return EXIT_FAILURE;
	}

	int n;
	char *tmp = malloc(256 * sizeof(char));
	while(1)
	{
		fprintf(stdout, "--------------------------\n");
		int len = 0;
		for(int i = 0; i < BUFF_SIZE; i++)
		{
			gets(tmp);
			// scanf("%[^\n]", tmp);
			// fprintf(stdout,"%s\n", tmp);
			if(!strcmp(tmp, "stop"))
			{
				kill(getppid(), SIGUSR1);
				return EXIT_SUCCESS;
			}
			tmp = append(tmp, '&');
			// len += snprintf(shm_buf+len, BUFF_SIZE-len, tmp);
			len += sprintf(shm_buf+len, tmp);
		}
		fprintf(stdout, "--------------------------\n");
		kill(getppid(), SIGUSR2);
		pause();
	}

	return EXIT_SUCCESS;
}

char *append(const char *s, char c)
{
    int len = strlen(s);
    char buf[len+2];
    strcpy(buf, s);
    buf[len] = c;
    buf[len + 1] = 0;
    return strdup(buf);
}

void sg_handler(int signo)
{
}
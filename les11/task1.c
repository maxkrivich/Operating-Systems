#include <getopt.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

int BUFF_SIZE = 3;
int shm_id;
int flag = 1;

void parsearg(int, char**);
void sg_handler(int);
void clean(void);

int main(int argc, char const *argv[])
{
	atexit(clean);
	parsearg(argc, argv);
	if(signal(SIGUSR1, sg_handler) == SIG_ERR)
	{
		fprintf(stderr, "\ncan't catch SIGUSR1\n");
		exit(EXIT_FAILURE);
	}
	if(signal(SIGUSR2, sg_handler) == SIG_ERR)
	{
		fprintf(stderr, "\ncan't catch SIGUSR1\n");
		exit(EXIT_FAILURE);
	}

	char* shm_buf;
	int shm_size;
	struct shmid_ds ds;
	key_t key;
	pid_t pid;

	shm_id = shmget(IPC_PRIVATE, BUFF_SIZE, IPC_CREAT | IPC_EXCL | 0600);
	if (shm_id == -1) 
	{
		fprintf(stderr, "shmget() error\n");
		return EXIT_FAILURE;
	}
	shm_buf = (char *) shmat(shm_id, NULL, 0);
	if (shm_buf == (char *) -1) 
	{
		fprintf(stderr, "shmat() error\n");
		return EXIT_FAILURE;
	}

	pid = fork();

	if(pid == 0)
	{
			char a[10];
			char b[10];
			sprintf(a,"%d", shm_id);
			sprintf(b,"%d", BUFF_SIZE);

			if(!flag)
			{
				char *argvv[] = {"./main", b, NULL};
				execve("./main", argvv, NULL);
			}
			else
			{
				char *argvv[] = {"./main", a, b, NULL};
				execve("./main", argvv, NULL);
			}
			
	}

	if(!flag)
	{
		pause();
		union sigval v;
		v.sival_int = shm_id;
		sigqueue(pid, SIGUSR1, v);
	}

	while(1)
	{
		pause();
		fprintf(stdout, "SUM: %d\n", sum(shm_buf));
		kill(pid, SIGUSR2);
	}

	// shmctl(shm_id, IPC_RMID, NULL);
		
	return EXIT_SUCCESS;
}


int sum(char * stream)
{
	char *cmp = strdup(stream);
	int ans = 0;
	for(char *pt = strtok(cmp, "&"); pt != NULL; pt = strtok(NULL, "&"))
		ans += atoi(pt);
	return ans;
}

void clean(void)
{
	char cmd[30];
	sprintf(cmd,"ipcrm -m %d", shm_id);
	system(cmd);
}

void sg_handler(int signo)
{
	if(signo == SIGUSR1)// exit
		exit(-1);
	else
		return;
}

void parsearg(int argc, char** argv)
{
	int res, ind;
	const char * short_opt = "hb:";
	const struct option long_opt[] = {
		{"help", no_argument, NULL, 'h'},
		{"buff", required_argument, NULL, 'b'},
		{NULL, 0, NULL, 0}
	};	
	while((res = getopt_long(argc, argv, short_opt, long_opt, &ind)) != -1)
	{
		switch(res)
		{
			case 'h':
				fprintf(stdout, "-b | --buff [int]\tbuffer size\n");
				break;
			case 'b':
				BUFF_SIZE = atoi(optarg);
				break;
			case '?':
				break;
			default:
				break;
		}
	}
}
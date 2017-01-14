#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

#define SEMNUM 3
#define SEM_KEY 2015
#define SHM_KEY 2015

int BUFF_SIZE = 3;

void parsearg(int, char**);
void sg_handler(int);
void clean(void);

union semnum {
	int val;
	struct semid_ds * buf;
	unsigned short * array;
} sem_arg;

int main(int argc, char *argv[])
{
	atexit(clean);
	parsearg(argc, argv);

	char* shm_buf;
	int shm_id;
	int sem_id;
	int shm_size;
	key_t key;
	pid_t pid;
	struct shmid_ds ds;
	struct sembuf sb[SEMNUM];
	unsigned short sem_vals[SEMNUM];

	shm_id = shmget(SHM_KEY, SHMEM_SIZE, IPC_CREAT | IPC_EXCL | 0600);
	if (shm_id == -1) 
	{
		fprintf(stderr, "shmget() error\n");
		return EXIT_FAILURE;
	}


	sem_id = semget(SEM_KEY, SEMNUM, 0600 | IPC_CREAT | IPC_EXCL);
	if (sem_id == -1) 
	{
		fprintf(stderr, "semget() error\n");
		return EXIT_FAILURE;
	}

	printf("Semaphore: %d\n", sem_id);
	sem_vals[0] = 1;
	sem_arg.array = sem_vals;
	if (semctl(sem_id, 0, SETALL, sem_arg) == -1) 
	{
		fprintf(stderr, "semctl() error\n");
		return EXIT_FAILURE;
	}

	shm_buf = (char *) shmat(shm_id, NULL, 0);
	if (shm_buf == (char *) -1) 
	{
		fprintf(stderr, "shmat() error\n");
		return EXIT_FAILURE;
	}

	shmctl(shm_id, IPC_STAT, &ds);
	shm_size = ds.shm_segsz;

	sb[0].sem_num = 1;
	sb[0].sem_op = 1;
    sb[0].sem_flg = 0;


    sb[1].sem_num = BUFF_SIZE;
    sb[1].sem_op = 1;
    sb[1].sem_flg = 0;

    sb[2].sem_num = 0;
    sb[2].sem_op = 1;
    sb[2].sem_flg = 0;


	pid = fork();

	
	return EXIT_SUCCESS;
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
#include "shared.h"

int get_buffer_size(char *sbuff) {
  int i = 0;
  while(sbuff[i] != '^' && i < BUFFER_SIZE) i++;
  // printf("ret = %d\n", i);
  return i;
}

void debug_buffer(char *sbuff) {
  int i = 0; 
  while(sbuff[i] != '^' && i < BUFFER_SIZE)  printf("%c", sbuff[i++]);
  printf("\n");
}

char *create_shared_mem_buffer() {
  char *shmaddr; 
  key_t key = SHM_KEY; 
  
  int shmid = shmget(key, BUFFER_SIZE, IPC_CREAT | SHM_R | SHM_W);
  if (errno > 0) {
    perror("failed to create shared memory segment");
    exit (EXIT_FAILURE);
  }

  shmaddr = (char*)shmat(shmid, NULL, 0);
  if (errno > 0) {
    perror ("failed to attach to shared memory segment");
    exit (EXIT_FAILURE);
  }
  return shmaddr;
}

void clear_buffer(char *sbuff) {
  sbuff[0] = '^';
  // int i = 0;
  // for (i = 0; i < BUFFER_SIZE; ++i) sbuff[i] = ' ';
}

int create_semaphore_set() {
  key_t key = ftok(SEM_KEY, 'E');
  
  int semid = semget(key, NSEM_SIZE, 0600 | IPC_CREAT);
  if (errno > 0) {
    perror("failed to create semaphore array");
    exit (EXIT_FAILURE);
  } 

  semctl(semid, FULL_ID, SETVAL, 0);
  if (errno > 0) {
    perror("failed to set FULL semaphore");
    exit (EXIT_FAILURE);
  }

  semctl(semid, EMPTY_ID, SETVAL, BUFFER_SIZE);
  if (errno > 0) {
    perror("failed to set EMPTY sempahore");
    exit (EXIT_FAILURE);
  }

  semctl(semid, MUTEX_ID, SETVAL, 1);
  if (errno > 0) {
    perror("failed to create mutex");
  }

  return semid;
}

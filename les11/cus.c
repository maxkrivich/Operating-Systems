#include "shared.h"

int sum = 0;

void consume_item(int item) {
	if(item != -1)
 		sum += item;
 	printf("sum:\t%d\n", sum);
}

int remove_item(int semid, char *shared_buffer) {
  int index = get_buffer_size(shared_buffer);
  if(isdigit(shared_buffer[index - 1]))
  {
      int ans = shared_buffer[index - 1] - '0';
      shared_buffer[index - 1] = '^';
      return ans;
  }
 else
 	    return -1;
}

int main(int argc, const char *argv[])
{
  char *shared_buffer = create_shared_mem_buffer();
  int semid = create_semaphore_set();

  int item = 0;

  while(1) {
    semop(semid, &downFull, 1);
    semop(semid, &downMutex, 1);
    item = remove_item(semid, shared_buffer);
    debug_buffer(shared_buffer);
    semop(semid, &upMutex, 1);
    semop(semid, &upEmpty, 1);
    consume_item(item);
  } 

  return EXIT_SUCCESS;
}

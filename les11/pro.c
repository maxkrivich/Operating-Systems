#include "shared.h"

void insert_item(char item, int semid, char *shared_buffer) {
  int index = get_buffer_size(shared_buffer);
  shared_buffer[index] = item;
  shared_buffer[index+1] = '^';
}


// char *append(const char *s, char c)
// {
//     int len = strlen(s);
//     char *buf = malloc(sizeof(char)* (len +1));
//     strcpy(buf, s);
//     buf[len] = c;
//     // buf[len + 1] = 0;
//     return strdup(buf);
// }

char produce_item() {
  fprintf(stdout, "INPUT DIGIT:");
  char tmp[255];
  gets(tmp);
  return tmp[0]; // nothing dynamic just write a static integer a slot
}

int main(int argc, const char *argv[])
{
  char *shared_buffer = create_shared_mem_buffer();
  int semid = create_semaphore_set();

  clear_buffer(shared_buffer); 

  char item;

  while(1) {
    item = produce_item();
    // printf("\n%c\n", item);
    semop(semid, &downEmpty, 1);
    semop(semid, &downMutex, 1);
    insert_item(item, semid, shared_buffer);
    // debug_buffer(shared_buffer);
    semop(semid, &upMutex, 1);
    semop(semid, &upFull, 1);
  }
 
  return EXIT_SUCCESS;
}

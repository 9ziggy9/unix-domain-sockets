#include <fcntl.h>    // For O_* constants
#include <sys/mman.h> // For shared memory
#include <unistd.h>   // For ftruncate
#include <stdio.h>    // For printf/puts
#include <stdlib.h>   // For EXIT_FAILURE
#include <string.h>
#include <signal.h>

static const size_t MEM_SIZE = 4096;
static const char *POOL_NAME = "/mem_pool";

void inform_and_panic(char *source) {
  perror(source);
  exit(EXIT_FAILURE);
}

int main() {
  int shm_fd = shm_open(POOL_NAME, O_RDONLY, 0666);
  if (shm_fd == -1) inform_and_panic("shm_open");

  char *m_loc = mmap(0, MEM_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
  if (m_loc == MAP_FAILED) inform_and_panic("mmap");

  while (1) {
    printf("%s", m_loc);
    fflush(stdout);
    sleep(1);
  }

  return 0;
}

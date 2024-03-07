#include <fcntl.h>    // For O_* constants
#include <sys/mman.h> // For shared memory
#include <unistd.h>   // For ftruncate
#include <stdio.h>    // For printf/puts
#include <stdlib.h>   // For EXIT_FAILURE
#include <semaphore.h>
#include <string.h>
#include <signal.h>

// Dislike globals but won't worry for now...
static void *M_LOC = NULL;
static int SHM_FD = -1;
static const size_t MEM_SIZE = 4096;
static const char *POOL_NAME = "/mem_pool";
/* static const char *SEM_NAME  = "/mem_sem" */ //todo

void handle_sigint(int sig) {
  // You should check for SIG_ERR
  printf("\n");
  printf("Received signal: %d. Unmapping shared memory and cleaning...", sig);
  if (M_LOC != NULL) {
    munmap(M_LOC, MEM_SIZE);
    M_LOC = NULL;
  }
  if (SHM_FD != -1) {
    close(SHM_FD);
    shm_unlink(POOL_NAME); // Assuming you want to unlink as well
    SHM_FD = -1;
  }
  exit(EXIT_SUCCESS);
}

void inform_and_panic(char *source) {
  perror(source);
  exit(EXIT_FAILURE);
}

void broadcast_ran_num(char *origin) {
  while (1) {
    // Handle potential overflows
    sprintf(M_LOC, "%s: %d\n", origin, (rand() % 10) + 1);
    sleep(1);
  }
}

#define SZ_ARG_BUFFER 32
int main(int argc, char **argv) {
  // signal handler to intercept SIGINT and clean up gracefully.
  signal(SIGINT, handle_sigint);

  if (argc < 2) {
    printf("Please provide a name for the pool host.\n");
    exit(EXIT_FAILURE);
  }

  char originator[SZ_ARG_BUFFER];
  strncpy(originator, argv[1], SZ_ARG_BUFFER);
  originator[SZ_ARG_BUFFER] = '\0';

  SHM_FD = shm_open(POOL_NAME, O_CREAT | O_RDWR, 0666);
  if (SHM_FD == -1) inform_and_panic("shm_open");

  if (ftruncate(SHM_FD, (__off_t) MEM_SIZE) == -1) inform_and_panic("ftruncate");

  M_LOC = mmap(0, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, SHM_FD, 0);
  if (M_LOC == MAP_FAILED) inform_and_panic("mmap");

  broadcast_ran_num(originator);

  exit(EXIT_SUCCESS); // unreachable BTW.
}

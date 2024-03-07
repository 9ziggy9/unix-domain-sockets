#include "uconnect.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define SOCKET_PATH "./test.sock"
#define IN_BUFFER_SIZE 256

int main(int argc, char **argv) {
  int host_fd, client_fd;
  struct sockaddr_un host_addr;
  char buffer[BUFFER_SIZE];
  char hostname[IN_BUFFER_SIZE];

  if (argc < 2) {
    fprintf(stderr, "Please provide hostname as argument.\n");
    exit(EXIT_FAILURE);
  }

  strncpy(hostname, argv[1], IN_BUFFER_SIZE);
  hostname[IN_BUFFER_SIZE - 1] = '\0';

  init_msg(hostname, SOCKET_PATH);

  // Create socket
  host_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (host_fd == -1) {
    inform_and_panic("PANIC: socket not created, missing file descriptor.\n");
  }

  // Remove the existing file and bind socket to the socket path
  unlink(SOCKET_PATH);

  setup_host_address(&host_addr, SOCKET_PATH);

  bind_on_host(host_fd, &host_addr);

  listen_on_host(host_fd, SOCKET_PATH);

  accept_client_connection(host_fd, &client_fd);

  // Emit a random number between 1 and 10 every second
  while (1) {
      int number = (rand() % 10) + 1;
      snprintf(buffer, BUFFER_SIZE, "%s: %d\n", hostname, number);
      write(client_fd, buffer, strlen(buffer));
      sleep(1);
  }

  host_exit(host_fd, client_fd, SOCKET_PATH);

  return 0;
}

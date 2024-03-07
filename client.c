#include "uconnect.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#define SOCKET_PATH "./test.sock"

int main() {
  int client_fd;
  struct sockaddr_un host_addr;
  char buffer[BUFFER_SIZE];

  init_msg("client", SOCKET_PATH);

  // Create socket
  client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (client_fd == -1) {
    inform_and_panic("PANIC: socket not created, missing file descriptor.\n");
  }

  setup_host_address(&host_addr, SOCKET_PATH);

  connect_to_host(client_fd, &host_addr);

  int flags = fcntl(client_fd, F_GETFL, 0);
  fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);

  while (1) {
      ssize_t bytes_read = read(client_fd, buffer, BUFFER_SIZE);
      if (bytes_read == -1 && errno == EAGAIN) {
          // No data available, sleep for a bit and try again
          sleep(1);
          continue;
      }
      if (bytes_read == -1) {
          inform_and_panic("PANIC: unable to read from server.");
      }
      write(STDOUT_FILENO, buffer, (size_t) bytes_read);
  }

  // Close socket
  close(client_fd);

  return 0;
}

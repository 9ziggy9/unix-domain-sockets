#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "./test.sock"
#define BUFFER_SIZE 1024

void init_msg() {
  printf("Connecting to UNIX domain socket host @ %s...\n", SOCKET_PATH);
}

void inform_and_panic(const char *msg) {
  // Be absolutely certain no syscalls happen in between here!
  perror(msg);
  printf("Aborting!");
  exit(EXIT_FAILURE);
}

int main() {
  int client_fd;
  struct sockaddr_un server_addr;
  char buffer[BUFFER_SIZE];

  init_msg();

  // Create socket
  client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (client_fd == -1) {
    inform_and_panic("PANIC: socket not created, missing file descriptor.\n");
  }

  // Set up server address
  memset(&server_addr, 0, sizeof(struct sockaddr_un));
  server_addr.sun_family = AF_UNIX;
  strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

  // Connect to server
  if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_un)) == -1) {
    inform_and_panic("PANIC: unable to connect to server.");
  }

  // Receive data from server and echo back to standard out
  while (1) {
      int bytes_read = read(client_fd, buffer, BUFFER_SIZE);
      if (bytes_read == -1) {
          inform_and_panic("PANIC: unable to read from server.");
      }
      write(STDOUT_FILENO, buffer, bytes_read);
  }

  // Close socket
  close(client_fd);

  return 0;
}

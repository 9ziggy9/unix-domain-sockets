#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "./test.sock"
#define BUFFER_SIZE 1024

void init_msg() {
  printf("Launching UNIX domain socket host @ %s...\n", SOCKET_PATH);
}

void inform_and_panic(const char *msg) {
  // Be absolutely certain no syscalls happen inbetween here!
  perror(msg);
  printf("Aborting!");
  exit(EXIT_FAILURE);
}

int main() {
  int host_fd, client_fd;
  struct sockaddr_un server_addr;
  char buffer[BUFFER_SIZE];

  init_msg();

  // Create socket
  host_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (host_fd == -1) {
    inform_and_panic("PANIC: socket not created, missing file descriptor.\n");
  }

  // Remove the existing file and bind socket to the socket path
  unlink(SOCKET_PATH);
  memset(&server_addr, 0, sizeof(struct sockaddr_un));
  server_addr.sun_family = AF_UNIX;
  strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

  if (bind(host_fd, (struct sockaddr *)&server_addr,
           sizeof(struct sockaddr_un)) == -1) {
    inform_and_panic("PANIC: bind to host file descriptor fail.\n");
  }

  // Listen for incoming connections
  if (listen(host_fd, 5) == -1) {
    inform_and_panic("PANIC: unable to listen for incoming connections.");
  }

  printf("Server listening on %s...\n", SOCKET_PATH);

  // Accept client connection
  client_fd = accept(host_fd, NULL, NULL);
  if (client_fd == -1) {
    inform_and_panic("PANIC: client connection failed.");
  }

  // Emit a random number between 1 and 10 every second
  while (1) {
      int number = (rand() % 10) + 1;
      snprintf(buffer, BUFFER_SIZE, "%d\n", number + '0');
      write(client_fd, buffer, strlen(buffer));
      sleep(1);
  }

  // Close sockets
  close(client_fd);
  close(host_fd);
  unlink(SOCKET_PATH);

  return 0;
}

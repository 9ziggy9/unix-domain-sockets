#include "uconnect.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void init_msg(const char *host_type, const char *sock_path) {
  printf("Launching UNIX domain socket %s @ %s...\n", host_type, sock_path);
}

void inform_and_panic(const char *msg) {
  // Be absolutely certain no syscalls happen in between here!
  perror(msg);
  printf("Aborting!");
  exit(EXIT_FAILURE);
}

void setup_host_address(struct sockaddr_un *host_addr, const char *sock_path) {
  memset(host_addr, 0, sizeof(struct sockaddr_un));
  host_addr->sun_family = AF_UNIX;
  strncpy(host_addr->sun_path, sock_path, sizeof(host_addr->sun_path) - 1);
}

void connect_to_host(int client_fd, struct sockaddr_un *host_addr) {
  if (connect(client_fd,
              (struct sockaddr *)host_addr,
              sizeof(struct sockaddr_un)) == -1) {
    inform_and_panic("PANIC: unable to connect to host.");
  }
}

void listen_on_host(int host_fd, const char *sock_path) {
  if (listen(host_fd, 5) == -1) {
    inform_and_panic("PANIC: unable to listen for incoming connections.");
  }
  printf("Server listening on %s...\n", sock_path);
}

void bind_on_host(int host_fd, struct sockaddr_un *host_addr) {
  if (bind(host_fd, (struct sockaddr *) host_addr,
           sizeof(struct sockaddr_un)) == -1) {
    inform_and_panic("PANIC: bind to host file descriptor fail.\n");
  }
}

void accept_client_connection(int host_fd, int *client_fd) {
  *client_fd = accept(host_fd, NULL, NULL);
  if (client_fd == -1) {
    inform_and_panic("PANIC: client connection failed.");
  }
}

void host_exit(int host_fd, int client_fd, const char *sock_path) {
  close(client_fd);
  close(host_fd);
  unlink(sock_path);
}

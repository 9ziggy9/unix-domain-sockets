#ifndef UCONNECT_H
#define UCONNECT_H

#include <sys/socket.h>
#include <sys/un.h>

#define BUFFER_SIZE 1024

void init_msg(const char *host_type, const char *sock_path);
void inform_and_panic(const char *msg);
void setup_host_address(struct sockaddr_un *server_addr, const char *sock_path);
void connect_to_host(int client_fd, struct sockaddr_un *server_addr);
void listen_on_host(int host_fd, const char *sock_path);
void bind_on_host(int host_fd, struct sockaddr_un *host_addr);
void accept_client_connection(int host_fd, int *client_fd);
void host_exit(int host_fd, int client_fd, const char *sock_path);

#endif // UCONNECT_H

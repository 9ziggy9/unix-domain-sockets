#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    socklen_t clilen;
    int n;
    int random_integer;

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Set address of server
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket with address and port
    bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    // Listen for incoming connections
    listen(sockfd, 3);

    printf("Server is listening...\n");

    while (1) {
        // Accept incoming connection
        clilen = sizeof(cli);
        connfd = accept(sockfd, (struct sockaddr *)&cli, &clilen);

        // Generate a random integer in ASCII numerical range
        random_integer = rand() % 10 + '0';

        // Send the random integer to the client
        n = send(connfd, &random_integer, sizeof(random_integer), 0);
        if (n < 0) {
            perror("send");
            exit(1);
        }

        // Log incoming traffic
        char buffer[1024];
        n = recv(connfd, buffer, sizeof(buffer), 0);
        if (n < 0) {
            perror("recv");
            exit(1);
        }
        printf("Received message from client: %s\n", buffer);

        // Close the connection
        close(connfd);
    }

    return 0;
}

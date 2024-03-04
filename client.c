#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    socklen_t clilen;
    char buffer[1024];

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Set address of server
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // Connect to server
    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    printf("Client connected to server...\n");

    // Receive and log random numbers from server
    while (1) {
        recv(sockfd, buffer, sizeof(buffer), 0);
        printf("Random number from server: %c\n", buffer[0]);
    }

    return 0;
}

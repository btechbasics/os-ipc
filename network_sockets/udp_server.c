#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080

int main() {
    struct sockaddr_in server_addr, client_addr;
    int server_sock;
    socklen_t client_len;
    char buffer[256];

    // Create a UDP socket. Note the change from SOCK_STREAM to SOCK_DGRAM.
    server_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_sock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // The bind call remains the same for both TCP and UDP.
    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // For UDP, there's no listen() or accept() as it's a connectionless protocol.
    client_len = sizeof(client_addr);

    // Change from read() to recvfrom() for UDP. 
    recvfrom(server_sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &client_len);
    printf("Received: %s\n", buffer);

    close(server_sock);

    return 0;
}
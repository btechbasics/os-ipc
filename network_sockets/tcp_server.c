#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080

int main() {
    struct sockaddr_in server_addr, client_addr;
    int server_sock, client_sock;
    socklen_t client_len;
    // buffer to store message from client
    char buffer[256];

    // Create a TCP socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    // In case INAADDR_ANY is used, the server will listen on all interfaces.
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket to port 8080 of localhost
    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    listen(server_sock, 5);
    client_len = sizeof(client_addr);

    // Accept connection from client
    client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_len);

    // Read message from client
    read(client_sock, buffer, sizeof(buffer));
    printf("Received: %s\n", buffer);

    // Close sockets
    close(client_sock);
    close(server_sock);

    return 0;
}

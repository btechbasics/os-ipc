#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

// Path to socket file
#define SOCKET_PATH "unix_socket"

int main() {
    struct sockaddr_un server_addr;
    int server_sock, client_sock;
    char *message = "Hello from server!";
    char buffer[256];

    // Create a UNIX socket
    server_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    // Copy socket path to server_addr
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    // Bind socket to socket file
    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    listen(server_sock, 5);

    // Accept connection from client
    client_sock = accept(server_sock, NULL, NULL);

    // Read message from client
    read(client_sock, buffer, sizeof(buffer));
    printf("Received: %s\n", buffer);

    // Send message to client
    write(client_sock, message, strlen(message) + 1);

    // Close sockets
    close(client_sock);

    // Close socket file
    close(server_sock);

    // Delete socket file
    unlink(SOCKET_PATH);

    return 0;
}

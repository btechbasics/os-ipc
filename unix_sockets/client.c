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
    int client_sock;
    // message to send to server
    char *message = "Hello from client!";

    // Create a UNIX socket
    client_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_sock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    // Copy socket path to server_addr
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    // Connect to server
    if (connect(client_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    // Send message to server
    write(client_sock, message, strlen(message) + 1);

    char buffer[256];
    // Read message from server
    read(client_sock, buffer, sizeof(buffer));
    printf("Received: %s\n", buffer);

    // Close socket
    close(client_sock);
    return 0;
}

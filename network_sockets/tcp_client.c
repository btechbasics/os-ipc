#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080

int main() {
    struct sockaddr_in server_addr;
    int client_sock;
    // message to send to server
    char *message = "Hello from TCP client!";

    // Create a TCP socket
    client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    
    // As no IP is defeined in client case, it will connect to localhost.
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Connect to server, which is listening on localhost:8080
    if (connect(client_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    // Send message to server
    write(client_sock, message, strlen(message) + 1);

    // Close socket
    close(client_sock);
    return 0;
}

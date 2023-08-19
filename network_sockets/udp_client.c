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
    char *message = "Hello from UDP client!";

    // Create a UDP socket. Note the change from SOCK_STREAM to SOCK_DGRAM.
    client_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_sock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // There's no connect() for UDP, as it's connectionless.
    // However, you can still use connect() with UDP, not for actual connections,
    // but to fix the peer address for subsequent sends/receives.
    // If you use connect() here, you can use send() and recv() calls just like in TCP.
    // But, for this example, we'll omit it.

    // Change from write() to sendto() for UDP.
    sendto(client_sock, message, strlen(message) + 1, 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

    close(client_sock);
    return 0;
}
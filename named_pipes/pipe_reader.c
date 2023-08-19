#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    int fd;
    char * myfifo = "/tmp/myfifo";
    char readbuffer[100];

    // Open the FIFO for read only
    fd = open(myfifo, O_RDONLY);

    // Read data from the FIFO
    read(fd, readbuffer, sizeof(readbuffer));
    printf("Reader received: %s\n", readbuffer);

    // Close the FIFO
    close(fd);

    return 0;
}

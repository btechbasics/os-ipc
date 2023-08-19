#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    int fd;
    char * myfifo = "/tmp/myfifo";
    char string[] = "Hello, this is IPC using named pipes!";

    // Creating the named pipe (FIFO)
    mkfifo(myfifo, 0666);

    // Open the FIFO for write only
    fd = open(myfifo, O_WRONLY);

    // Write data to the FIFO
    write(fd, string, sizeof(string));

    // Close the FIFO
    close(fd);

    // Delete the FIFO
    unlink(myfifo);

    return 0;
}

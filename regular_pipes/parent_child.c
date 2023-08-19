#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
    int fd[2];  // File descriptors for the pipe
    pid_t childpid;
    // Message to be written to the pipe
    char string[] = "Hello, this is IPC using pipes!";
    // Buffer to store read data
    char readbuffer[100];

    // Create the pipe
    if (pipe(fd) == -1) {
        perror("Failed to create the pipe");
        return 1;
    }

    // Fork a child process
    if ((childpid = fork()) == -1) {
        perror("Failed to fork");
        return 1;
    }

    // Child and parent processes will execute this block of code
    // Parent process will have childpid > 0, while child process will have childpid = 0
    // Parent will write into the pipe, while child will read from the pipe

    if (childpid == 0) {  // This block will be executed by the child
        close(fd[1]);  // Child closes the write end, as it's not needed

        // Read data from the pipe
        read(fd[0], readbuffer, sizeof(readbuffer));
        printf ("\n\nChild PID : %d \n",getpid());
        printf("Child reads: %s\n", readbuffer);

        // Close the read end of the pipe
        close(fd[0]);
    } else {  // This block will be executed by the parent
        close(fd[0]);  // Parent closes the read end, as it's not needed

        printf ("\n\nParent PID : %d \n",getpid());
        printf("Parent writes: %s\n", string);
        
        // Write data to the pipe
        write(fd[1], string, (strlen(string) + 1));

        // Close the write end of the pipe
        close(fd[1]);

        // Wait for the child to finish
        wait(NULL);
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

// unique key for shared memory
#define SHM_KEY 1234

int main() {
    int shmid;              // shared memory ID
    int *counter;           // shared variable 
    int i;                 // loop counter

    // get shared memory ID, create if doesn't exist
    shmid = shmget(SHM_KEY, sizeof(int), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // attach shared memory segment to address space
    counter = (int *) shmat(shmid, NULL, 0);
    if (counter == (int *) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    *counter = 0;  // Initialize counter

    // fork child process
    // both child and parent increment counter upto 100000 times
    if (fork() == 0) {  // Child
        for (i = 0; i < 100000; i++) {
            (*counter)++;
        }
        shmdt(counter);
        exit(EXIT_SUCCESS);
    } else {  // Parent
        for (i = 0; i < 100000; i++) {
            (*counter)++;
        }

        // wait for child to finish
        wait(NULL);

        // As both child and parent increment counter upto 100000 times, the final value of counter should be 200000
        // However, as there is no concurrency control, the final value of counter is mostly won't be 200000
        printf("Expected value: 200000, Actual value: %d\n", *counter);
        
        // detach from shared memory
        shmdt(counter);

        // destroy the shared memory
        shmctl(shmid, IPC_RMID, NULL);
    }

    return 0;
}

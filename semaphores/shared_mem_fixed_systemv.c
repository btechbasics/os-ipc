#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

// unique key for shared memory
#define SHM_KEY 1234

// Define the union semun for semctl system call
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

// Initialize semaphore
int init_semaphore(int semid, int value) {
    union semun arg;
    arg.val = value;
    return semctl(semid, 0, SETVAL, arg);
}

// P operation (decrement)
int semaphore_P(int semid) {
    struct sembuf sb = {0, -1, 0};
    return semop(semid, &sb, 1);
}

// V operation (increment)
int semaphore_V(int semid) {
    struct sembuf sb = {0, 1, 0};
    return semop(semid, &sb, 1);
}

int main() {
    key_t key;
    int semid, shmid;
    int *counter;
    int i;

    // Generate a key for semaphore
    if ((key = ftok("/tmp", 'a')) == -1) {
        perror("ftok");
        exit(1);
    }

    // Create a semaphore set with a single semaphore
    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) == -1) {
        perror("semget");
        exit(1);
    }

    // Initialize semaphore to 1
    init_semaphore(semid, 1);

    // Create shared memory
    shmid = shmget(SHM_KEY, sizeof(int), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attach shared memory segment to address space
    counter = (int *) shmat(shmid, NULL, 0);

    // shmat() returns (void *) -1 on failure
    if (counter == (int *) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    *counter = 0;  // Initialize counter

    // Fork a child process
    pid_t pid = fork();

    if (pid == 0) { // Child process

        for (i = 0; i < 100000; i++) {
            semaphore_P(semid);
            (*counter)++;
            semaphore_V(semid);
        }

        // Detach from shared memory
        shmdt(counter);

        exit(EXIT_SUCCESS);

    } else if (pid > 0) { // Parent process

        for (i = 0; i < 100000; i++) {
            semaphore_P(semid);
            (*counter)++;
            semaphore_V(semid);
        }

        // Wait for child to finish
        wait(NULL);

        // As both child and parent increment counter upto 100000 times, the final value of counter should be 200000
        printf("Expected value: 200000, Actual value: %d\n", *counter);

        // Detach from shared memory
        shmdt(counter);

        // Remove the semaphore
        if (semctl(semid, 0, IPC_RMID, NULL) == -1) {
            perror("semctl");
            exit(1);
        }

        shmctl(shmid, IPC_RMID, NULL);  // Clean up shared memory

    } else {
        perror("fork");
        exit(1);
    }

    return 0;
}

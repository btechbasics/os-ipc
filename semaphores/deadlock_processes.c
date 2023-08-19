#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

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

// Main function
int main() {
    key_t keyA, keyB;
    int semidA, semidB;

    // Generate keys for System V semaphore A and B
    if ((keyA = ftok("/tmp", 'A')) == -1 || (keyB = ftok("/tmp", 'B')) == -1) {
        perror("ftok");
        exit(1);
    }

    // semget returns the semaphore identifier associated with key
    if ((semidA = semget(keyA, 1, 0666 | IPC_CREAT)) == -1 ||
        (semidB = semget(keyB, 1, 0666 | IPC_CREAT)) == -1) {
        perror("semget");
        exit(1);
    }

    // Initialize semaphore A and B to 1
    init_semaphore(semidA, 1);
    init_semaphore(semidB, 1);

    // Create child process
    pid_t pid = fork();

    if (pid == 0) { // Child process
    
        semaphore_P(semidB);                        // Lock semaphore B

        printf("Child acquired semaphore B\n");

        /* <---------- Critical section starts ----------> */

        // Shared memory will be accessed here
        
        sleep(2);
        
        printf("Child waiting to acquire semaphore A\n");
        
        semaphore_P(semidA);
        
        printf("Child acquired semaphore A\n");

        /* ---------- Critical section ends ----------> */

        semaphore_V(semidA);                      // Unlock semaphore A
        semaphore_V(semidB);                      // Unlock semaphore B
        
        exit(0);
    } else if (pid > 0) { // Parent process

        semaphore_P(semidA);

        printf("Parent acquired semaphore A\n");
        
        /* <---------- Critical section starts ----------> */
        
        // Shared memory will be accessed here
        sleep(2);

        printf("Parent waiting to acquire semaphore B\n");
        
        semaphore_P(semidB);
        
        printf("Parent acquired semaphore B\n");

        /* ---------- Critical section ends ----------> */

        semaphore_V(semidB);                     // Unlock semaphore B
        semaphore_V(semidA);                    // Unlock semaphore A

    } else {
        perror("fork");
        exit(1);
    }

    // Wait for child process to terminate
    wait(NULL);

    // Cleanup
    union semun arg;
    semctl(semidA, 0, IPC_RMID, arg); // Remove semaphore A
    semctl(semidB, 0, IPC_RMID, arg); // Remove semaphore B

    return 0;
}

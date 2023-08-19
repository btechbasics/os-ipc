#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>

// unique key for semaphore
#define SEM_KEY 5678

// unique key for shared memory
#define SHM_KEY 1234

// Initialize semaphore
void init_semaphore(int semid) {

    // semun union structure
    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    } argument;

    argument.val = 1;
    
    // semctl() performs various operations on the semaphore
    if (semctl(semid, 0, SETVAL, argument) == -1) {
        perror("semctl");
        exit(EXIT_FAILURE);
    }
}

// Lock semaphore (P operation)
void lock(int semid) {
    struct sembuf operations[1];
    operations[0].sem_num = 0;
    operations[0].sem_op = -1;
    operations[0].sem_flg = 0;
    semop(semid, operations, 1);
}

// Unlock semaphore (V operation)
void unlock(int semid) {
    struct sembuf operations[1];
    operations[0].sem_num = 0;
    operations[0].sem_op = 1;
    operations[0].sem_flg = 0;
    semop(semid, operations, 1);
}

// Main function
int main() {
    int shmid, semid;
    int *counter;
    int i;

    // Create shared memory
    shmid = shmget(SHM_KEY, sizeof(int), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Create semaphore
    semid = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    // Initialize semaphore
    init_semaphore(semid);

    // Attach shared memory segment to address space
    counter = (int *) shmat(shmid, NULL, 0);

    // shmat() returns (void *) -1 on failure
    if (counter == (int *) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    *counter = 0;  // Initialize counter

    // fork child process
    // both child and parent increment counter upto 100000 times
    if (fork() == 0) {  // Child

        for (i = 0; i < 100000; i++) {

            lock(semid);        // Lock semaphore

            (*counter)++;       // Critical section, increment counter
            
            unlock(semid);      // Unlock semaphore
        }

        // detach from shared memory
        shmdt(counter);

        exit(EXIT_SUCCESS);

    } else {  // Parent

        for (i = 0; i < 100000; i++) {

            lock(semid);        // Lock semaphore
            
            (*counter)++;       // Critical section, increment counter
            
            unlock(semid);      // Unlock semaphore
        }

        // wait for child to finish
        wait(NULL);

        // As both child and parent increment counter upto 100000 times, the final value of counter should be 200000
        printf("Expected value: 200000, Actual value: %d\n", *counter);

        // detach from shared memory
        shmdt(counter);

        shmctl(shmid, IPC_RMID, NULL);  // Clean up shared memory
        semctl(semid, 0, IPC_RMID);     // Clean up semaphore
    }

    return 0;
}

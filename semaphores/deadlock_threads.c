#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Declare semaphores
sem_t semA;
sem_t semB;

// Function for thread 1
void* func1(void* arg) {

    sem_wait(&semA);
    printf("Thread 1 acquired semaphore A\n");

    /* <---------- Critical section starts ----------> */

    // Shared memory will be accessed here
    sleep(1);

    printf("Thread 1 waiting to acquire semaphore B\n");

    sem_wait(&semB);
    printf("Thread 1 acquired semaphore B\n");

    /* ---------- Critical section ends ----------> */

    sem_post(&semB);    // Unlock semaphore B
    sem_post(&semA);    // Unlock semaphore A

    return NULL;
}

// Function for thread 2
void* func2(void* arg) {

    sem_wait(&semB);
    printf("Thread 2 acquired semaphore B\n");

    /* <---------- Critical section starts ----------> */

    // Shared memory will be accessed here
    sleep(1);

    printf("Thread 2 waiting to acquire semaphore A\n");

    sem_wait(&semA);
    printf("Thread 2 acquired semaphore A\n");

    /* ---------- Critical section ends ----------> */

    sem_post(&semA);    // Unlock semaphore A
    sem_post(&semB);    // Unlock semaphore B

    return NULL;
}

int main() {
    pthread_t t1, t2;

    // Initialize semaphores
    sem_init(&semA, 0, 1);
    sem_init(&semB, 0, 1);

    // Create threads to run func1() and func2() simultaneously
    pthread_create(&t1, NULL, func1, NULL);
    pthread_create(&t2, NULL, func2, NULL);

    // Wait for threads to terminate
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // Destroy semaphores
    sem_destroy(&semA);
    sem_destroy(&semB);

    return 0;
}

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

int main() {
    key_t key = ftok("shmfile", 65);  // unique key

    // shmget returns an identifier in shmid
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);

    // shmat to attach *str pointer to shared memory
    char *str = (char*) shmat(shmid, (void*)0, 0);

    // get input from user, store in shared memory
    printf("Write data: ");
    gets(str);

    // show content of shared memory
    printf("Data written in memory: %s\n", str);
    
    //detach from shared memory
    shmdt(str);

    return 0;
}

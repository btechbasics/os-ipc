#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "msg_structure.h"

int main() {
    // integer message queue identifier
    int msg_queue_id;
    // message structure
    struct message msg;

    // get message queue identifier, using MESSAGE_QUEUE_KEY (defined in msg_structure.h)
    // msgget function returns the message queue identifier associated with the value of key
    // IPC_CREAT | 0666 means create a new message queue if it doesn't exist
    msg_queue_id = msgget(MESSAGE_QUEUE_KEY, IPC_CREAT | 0666);

    // check for errors, exit if any
    if (msg_queue_id == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // receive message, exit if any error
    // msgrcv function receives a message from the message queue
    if (msgrcv(msg_queue_id, &msg, sizeof(msg.message_text), 1, 0) == -1) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }

    // print received message
    printf("Received message: %s", msg.message_text);

    return 0;
}

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
    if (msg_queue_id == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    msg.message_type = 1;  // Set the type of message
    
    // Get message from user
    printf("Enter a message: ");
    fgets(msg.message_text, sizeof(msg.message_text), stdin);

    // Send message, exit if any error
    // msgsnd function sends a message to the message queue
    if (msgsnd(msg_queue_id, &msg, sizeof(msg.message_text), 0) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }

    // Print message sent
    printf("Message sent!\n");

    return 0;
}

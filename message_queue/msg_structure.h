#ifndef MSG_STRUCTURE_H
#define MSG_STRUCTURE_H

#include <sys/types.h>

#define MESSAGE_QUEUE_KEY 1337  // A unique key to identify our message queue
#define MAX_MESSAGE_SIZE 256    // Maximum size of our messages

// Structure of our messages
struct message {
    long message_type;              // This needs to be of type long and is mandatory
    char message_text[MAX_MESSAGE_SIZE];
};

#endif

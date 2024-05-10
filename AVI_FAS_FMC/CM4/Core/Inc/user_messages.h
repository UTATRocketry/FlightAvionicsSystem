#ifndef USER_MESSAGES_H
#define USER_MESSAGES_H

#include <stdint.h>

/* Message buffer properties properties */
#define USER_MESSAGES_BUFFER_LEN 256
#define USER_MESSAGES_QUEUE_SIZE 8
#define USER_MESSAGES_OVERFLOW_MESSAGE ("User Message Buffer Overflow!")
#define USER_MESSAGES_TOO_LONG_MESSAGE ("User Message Too Long!")

typedef struct user_message {
    char buffer[USER_MESSAGES_BUFFER_LEN];
} user_message;

typedef struct user_message_buffer_queue {
    user_message messages[USER_MESSAGES_QUEUE_SIZE];
    uint8_t head;
    uint8_t tail;
} user_message_buffer_queue;

/* Functions to implement */
void user_messages_initialize();

void user_messages_enqueue(uint8_t* str);

// The input struct message will become a copy of the dequeued message, to be overwritten
void user_messages_dequeue(user_message message);

#endif /* USER_MESSAGES_H */
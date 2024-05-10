#include "user_messages.h"

/* Global struct */
user_message_buffer_queue message_queue;

void user_messages_initialize() {
    message_queue.head = 0;
    message_queue.tail = 0;
}

void user_messages_enqueue(uint8_t* str) {
}

void user_messages_dequeue(user_message message) {

}
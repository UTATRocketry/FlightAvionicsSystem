#ifndef USER_MESSAGES_H
#define USER_MESSAGES_H

#include "cmsis_os2.h"
#include <stdint.h>

/* Message buffer properties properties */
#define USER_MESSAGES_BUFFER_LEN (256)
#define USER_MESSAGES_QUEUE_SIZE (8)
#define USER_MESSAGES_MUTEX_WAIT (50)
#define USER_MESSAGES_OVERFLOW_MESSAGE ("User Message Buffer Overflow!\0")
#define USER_MESSAGES_TOO_LONG_MESSAGE ("User Message Too Long!\0")

// Note: more properties may be useful here
typedef struct user_message {
    char buffer[USER_MESSAGES_BUFFER_LEN];
} user_message;

// Note: the mutex and semaphore may not both be necessary
typedef struct user_message_buffer_queue {
    user_message messages[USER_MESSAGES_QUEUE_SIZE];
    uint8_t head;
    uint8_t tail;
    uint8_t num_occupied;
    osMutexId_t mutex_handle;
    osSemaphoreId_t sem_handle;
} user_message_buffer_queue;

/* 
    Utility function errors
    Ideally, this would be contained in a separate .h function called errors.h implemented by every other file
*/
#define USER_MESSAGE_ERROR_SEMAPHORE_NOT_ACQUIRED (-1)
#define USER_MESSAGE_ERROR_SEMAPHORE_NOT_RELEASED (-2)

/* Declaring pointers for message buffer */
extern const osMutexAttr_t user_message_buffer_queue_mutex_attr;
extern const osSemaphoreAttr_t user_message_buffer_queue_sem_attr;

/* Declare semaphore for UART callbacks */
extern const osSemaphoreAttr_t usart3_available_sem_attr;
extern osSemaphoreId_t usart3_available_sem_handle;

// Note: these functions are not compelte yet
void usart3_available_sem_initialize();
int usart3_available_sem_post();
int usart3_available_sem_wait();

void user_messages_initialize();

int user_messages_enqueue(uint8_t* str);

// The input struct message will become a copy of the dequeued message, to be overwritten
int user_messages_wait_dequeue(user_message* message);

#endif /* USER_MESSAGES_H */
#include "../Include/user_messages.h"

#include <string.h>

/* Initialize pointers to mutex for message buffer */
const osMutexAttr_t user_message_buffer_queue_mutex_attr = {
    .name = "user_message_buffer_queue_mutex"
};
const osSemaphoreAttr_t user_message_buffer_queue_sem_attr = {
    .name = "user_message_buffer_queue_sem"
};

/* Declare semaphore for UART callbacks */
const osSemaphoreAttr_t usart3_available_sem_attr = {
    .name = "usart3_available_sem"
};



/* Global struct */
static user_message_buffer_queue message_queue;

void usart3_available_sem_initialize() {
  usart3_available_sem_handle = osSemaphoreNew(1, 1, &usart3_available_sem_attr);
}

int usart3_available_sem_post() {
  int status = osSemaphoreRelease(usart3_available_sem_handle);
  if (status != osOK)
  {
    return status;
  }
  return 0;
}

int usart3_available_sem_wait() {
  int status = osSemaphoreAcquire(usart3_available_sem_handle, osWaitForever);
  if (status != osOK)
  {
    return status;
  }
  return 0;
}

void user_messages_initialize() {
  message_queue.head = 0;
  message_queue.tail = 0;
  message_queue.num_occupied = 0;

  // Initialize mutex
  message_queue.mutex_handle = osMutexNew(&user_message_buffer_queue_mutex_attr);

  // Initialize semaphore
  message_queue.sem_handle = osSemaphoreNew(USER_MESSAGES_QUEUE_SIZE, 0, &user_message_buffer_queue_sem_attr);
}

int user_messages_enqueue(uint8_t* str) {
  // Acquire the mutex
  int status = osMutexAcquire(message_queue.mutex_handle, USER_MESSAGES_MUTEX_WAIT);
  if (status != osOK)
  {
    return status;
  }

  // Check if full
  if(message_queue.num_occupied == USER_MESSAGES_QUEUE_SIZE) {
      // Insert an error message
      // Other implementation options
      strcpy((message_queue.messages[message_queue.tail]).buffer, USER_MESSAGES_OVERFLOW_MESSAGE);
      return 1;
  }
  // Insert
  strcpy((message_queue.messages[message_queue.tail]).buffer, str);
  message_queue.tail = (message_queue.tail + 1) % USER_MESSAGES_QUEUE_SIZE;
  message_queue.num_occupied++;

  // Release the mutex
  status = osMutexRelease(message_queue.mutex_handle);
  if (status != osOK)
  {
    return status;
  }

  // Post to the semaphore
  status = osSemaphoreRelease(message_queue.sem_handle);
  if (status != osOK)
  {
    return status;
  }

  return 0;
}

int user_messages_wait_dequeue(user_message* message) {
  // Wait on the semaphore
  int status = osSemaphoreAcquire(message_queue.sem_handle, osWaitForever);
  if (status != osOK)
  {
    return status;
  }

  // Acquire the mutex
  status = osMutexAcquire(message_queue.mutex_handle, USER_MESSAGES_MUTEX_WAIT);
  if (status != osOK)
  {
    return status;
  }

  // Check if empty
  if(message_queue.num_occupied == 0) {
      // No need for an error message?
      return 1;
  }
  // Insert into the buffer field for input struct message
  strcpy(message->buffer, (message_queue.messages[message_queue.tail]).buffer);
  message_queue.head = (message_queue.head + 1) % USER_MESSAGES_QUEUE_SIZE;
  message_queue.num_occupied--;

  // Release the mutex
  status = osMutexRelease(message_queue.mutex_handle);
  if (status != osOK)
  {
    return status;
  }

  return 0;
}
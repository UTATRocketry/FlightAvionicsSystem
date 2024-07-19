#include "../Include/core_comms.h"

#include <stdint.h>
#include <string.h>

/* 
  Initialize pointers to shared memory structs
  Each message buffer needs a osMutexAttr_t upon initialization struct which contains basic details (i.e. label)
  the core_comm_channel struct contains all the data for the communication channel, including a mutex, data buffer, ACK bits, etc.
*/
const osMutexAttr_t comm_CM4_to_CM7_messages_mutex_attr = {
    .name = "comm_CM4_to_CM7_messages_mutex"
};
volatile core_comm_channel * const comm_CM4_to_CM7_messages_ptr = (core_comm_channel*)BUFF_CM4_TO_CM7_MESSAGES_ADDR;

const osMutexAttr_t comm_CM7_to_CM4_messages_mutex_attr = {
    .name = "comm_CM7_to_CM4_messages_mutex"
};
volatile core_comm_channel * const comm_CM7_to_CM4_messages_ptr = (core_comm_channel*)BUFF_CM7_TO_CM4_MESSAGES_ADDR;


/* 
  Initialization Functions
*/
void core_comms_init_all_channels(void) {
    core_comms_init_CM4_to_CM7_messages();
    core_comms_init_CM7_to_CM4_messages();
}

void core_comms_init_CM4_to_CM7_messages()
{
  comm_CM4_to_CM7_messages_ptr->mutex_handle = osMutexNew(&comm_CM4_to_CM7_messages_mutex_attr);
  memset((void *) comm_CM4_to_CM7_messages_ptr->buffer, 0, (size_t)CORE_COMM_CHANNEL_BUFFER_LEN);
  comm_CM4_to_CM7_messages_ptr->receiver_acknowledged = 1; // start off as ready to send
  comm_CM4_to_CM7_messages_ptr->ready_to_read = 0;
}

void core_comms_init_CM7_to_CM4_messages()
{
  comm_CM7_to_CM4_messages_ptr->mutex_handle = osMutexNew(&comm_CM7_to_CM4_messages_mutex_attr);
  memset((void *) comm_CM7_to_CM4_messages_ptr->buffer, 0, (size_t)CORE_COMM_CHANNEL_BUFFER_LEN);
  comm_CM7_to_CM4_messages_ptr->receiver_acknowledged = 1; // start off as ready to send
  comm_CM7_to_CM4_messages_ptr->ready_to_read = 0;
}


/*
  Utility functions
*/
int core_comms_channel_ready(volatile core_comm_channel* comm_ptr) {
  /*
    WIP => FIXME
    This requires some careful mapping out of which core / thread holds the mutex and all the concurrency issues that are involved
    The current implementation does not properly work
  */

  int val;

  int status = osMutexAcquire(comm_ptr->mutex_handle, CORE_COMM_MUTEX_WAIT);
  if (status != osOK)
  {
    return -1;
  }

  val = comm_ptr->ready_to_read;

  status = osMutexRelease(comm_ptr->mutex_handle);
  if (status != osOK)
  {
    return -1;
  }

  return val;
}

int core_comms_channel_acknowledged(volatile core_comm_channel* comm_ptr) {
  /*
    WIP => FIXME
    This requires some careful mapping out of which core / thread holds the mutex and all the concurrency issues that are involved
    The current implementation does not properly work
  */

  int val;

  int status = osMutexAcquire(comm_ptr->mutex_handle, CORE_COMM_MUTEX_WAIT);
  if (status != osOK)
  {
    return -1;
  }

  val = comm_ptr->receiver_acknowledged;

  status = osMutexRelease(comm_ptr->mutex_handle);
  if (status != osOK)
  {
    return -1;
  }

  return val;
}

int core_comms_channel_send(volatile core_comm_channel* comm_ptr, uint8_t* send_buffer, int buffer_len) {
  /*
    WIP => FIXME
    Ideally, this function would also ensure that the channel is ready for communication
    This functionality has not been added yet
  */
  int status = osMutexAcquire(comm_ptr->mutex_handle, CORE_COMM_MUTEX_WAIT);
  if (status != osOK)
  {
    return CORE_COMM_ERROR_MUTEX_NOT_ACQUIRED;
  }

  // Assert new message has not yet been received
  comm_ptr->receiver_acknowledged = 0;
  // Fill up the channel buffer
  // Can only send messages of max length CORE_COMM_CHANNEL_BUFFER_LEN
  int bytes_sent = 0;
  if(buffer_len < 0) {
    bytes_sent = CORE_COMM_CHANNEL_BUFFER_LEN;
  } else {
    bytes_sent = buffer_len;
  }
  strncpy((char *)comm_ptr->buffer, (char *) send_buffer, bytes_sent);
  // Assert new message is ready to be read
  comm_ptr->ready_to_read = 1;

  status = osMutexRelease(comm_ptr->mutex_handle);
  if (status != osOK)
  {
    return CORE_COMM_ERROR_MUTEX_NOT_RELEASED;
  }

  return bytes_sent;
}

int core_comms_channel_receive(volatile core_comm_channel* comm_ptr, uint8_t* rcv_buffer, int buffer_len) {
  /*
    WIP => FIXME
    Ideally, this function would also ensure that the channel is ready for communication
    This functionality has not been added yet
  */
  int status = osMutexAcquire(comm_ptr->mutex_handle, CORE_COMM_MUTEX_WAIT);
  if (status != osOK)
  {
    return CORE_COMM_ERROR_MUTEX_NOT_ACQUIRED;
  }

  // Assert message is read
  comm_ptr->ready_to_read = 0;
  // Fill up the receiver buffer
  // Can only receive messages of max length CORE_COMM_CHANNEL_BUFFER_LEN
  // Logically, this may not need to be here, considering messages longer than that can't be sent
  int bytes_rcvd;
  if(buffer_len < 0) {
    bytes_rcvd = CORE_COMM_CHANNEL_BUFFER_LEN;
  } else {
    bytes_rcvd = buffer_len;
  }
  strncpy((char *)rcv_buffer, (const char *) comm_ptr->buffer, bytes_rcvd);
  // Assert message has been received
  comm_ptr->receiver_acknowledged = 1;

  status = osMutexRelease(comm_ptr->mutex_handle);
  if (status != osOK)
  {
    return CORE_COMM_ERROR_MUTEX_NOT_RELEASED;
  }

  return bytes_rcvd;
}
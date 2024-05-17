#include "../Include/core_comms.h"

#include <stdint.h>
#include <string.h>

// Initialize pointers to shared memory structs
const osMutexAttr_t comm_CM4_to_CM7_messages_mutex_attr = {
    .name = "comm_CM4_to_CM7_messages_mutex"
};
volatile core_comm_channel * const comm_CM4_to_CM7_messages_ptr = (core_comm_channel*)BUFF_CM4_TO_CM7_MESSAGES_ADDR;

const osMutexAttr_t comm_CM7_to_CM4_messages_mutex_attr = {
    .name = "comm_CM7_to_CM4_messages_mutex"
};
volatile core_comm_channel * const comm_CM7_to_CM4_messages_ptr = (core_comm_channel*)BUFF_CM7_TO_CM4_MESSAGES_ADDR;

void core_comms_init_all_channels(void) {
    core_comms_init_CM4_to_CM7_messages();
    core_comms_init_CM7_to_CM4_messages();
}

void core_comms_init_CM4_to_CM7_messages()
{
  comm_CM4_to_CM7_messages_ptr->mutex_handle = osMutexNew(&comm_CM4_to_CM7_messages_mutex_attr);
  memset(comm_CM4_to_CM7_messages_ptr->buffer, 0, (size_t)CORE_COMM_CHANNEL_BUFFER_LEN);
  comm_CM4_to_CM7_messages_ptr->receiver_acknowledged = 1; // start off as ready to send
  comm_CM4_to_CM7_messages_ptr->ready_to_read = 0;
}

void core_comms_init_CM7_to_CM4_messages()
{
  comm_CM7_to_CM4_messages_ptr->mutex_handle = osMutexNew(&comm_CM7_to_CM4_messages_mutex_attr);
  memset(comm_CM7_to_CM4_messages_ptr->buffer, 0, (size_t)CORE_COMM_CHANNEL_BUFFER_LEN);
  comm_CM7_to_CM4_messages_ptr->receiver_acknowledged = 1; // start off as ready to send
  comm_CM7_to_CM4_messages_ptr->ready_to_read = 0;
}



int core_comms_channel_ready(volatile core_comm_channel* comm_ptr) {
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
  int status = osMutexAcquire(comm_ptr->mutex_handle, CORE_COMM_MUTEX_WAIT);
  if (status != osOK)
  {
    return -1;
  }

  comm_ptr->receiver_acknowledged = 0;
  if(buffer_len < 0) {
    strncpy(comm_ptr->buffer, send_buffer, CORE_COMM_CHANNEL_BUFFER_LEN);
  } else {
    strncpy(comm_ptr->buffer, send_buffer, buffer_len);
  }
  comm_ptr->ready_to_read = 1;

  status = osMutexRelease(comm_ptr->mutex_handle);
  if (status != osOK)
  {
    return -1;
  }

  return 0;
}

int core_comms_channel_receive(volatile core_comm_channel* comm_ptr, uint8_t* rcv_buffer, int buffer_len) {
  int status = osMutexAcquire(comm_ptr->mutex_handle, CORE_COMM_MUTEX_WAIT);
  if (status != osOK)
  {
    return -1;
  }

  comm_ptr->ready_to_read = 0;
  if(buffer_len < 0) {
    strncpy(rcv_buffer, comm_ptr->buffer, CORE_COMM_CHANNEL_BUFFER_LEN);
  } else {
    strncpy(rcv_buffer, comm_ptr->buffer, buffer_len);
  }
  comm_ptr->receiver_acknowledged = 1;

  status = osMutexRelease(comm_ptr->mutex_handle);
  if (status != osOK)
  {
    return -1;
  }

  return 0;
}
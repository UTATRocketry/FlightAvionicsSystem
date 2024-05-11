#include "../Include/core_comms.h"

// Initialize pointers to shared memory structs
const osMutexAttr_t comm_CM4_to_CM7_messages_mutex_attr = {
    .name = "comm_CM4_to_CM7_messages_mutex"
};
volatile core_comm_channel * const comm_CM4_to_CM7_messages_ptr = (core_comm_channel*)BUFF_CM4_TO_CM7_MESSAGES_ADDR;

void core_comms_init_all_channels(void) {
    core_comms_init_CM4_to_CM7_messages();
}

void core_comms_init_CM4_to_CM7_messages()
{
  comm_CM4_to_CM7_messages_ptr->mutexHandle = osMutexNew(&comm_CM4_to_CM7_messages_mutex_attr);
  memset(comm_CM4_to_CM7_messages_ptr->buffer, 0, CORE_COMM_CHANNEL_BUFFER_LEN);
}
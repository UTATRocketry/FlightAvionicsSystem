#ifndef CORE_COMMS_H
#define CORE_COMMS_H

#include "cmsis_os2.h"

/* Communication channel properties */
#define CORE_COMM_CHANNEL_BUFFER_LEN 512
#define CORE_COMM_MUTEX_WAIT 50

typedef struct core_comm_channel {
    uint8_t buffer[CORE_COMM_CHANNEL_BUFFER_LEN];
    osMutexId_t mutexHandle;
} core_comm_channel;

/* Align x to 4 bytes */
#define MEM_ALIGN(x) (((x) + 0x00000003) & ~0x00000003)

/* Shared RAM space to be used is SRAM4 in D3 domain */
#define SHARED_RAM_ADDR 0x38000000
#define SHARED_RAM_LEN  0x0000FFFF

/* Addresses of shared memory structs */
#define BUFF_CM4_TO_CM7_MESSAGES_ADDR MEM_ALIGN(SHARED_RAM_ADDR)
#define BUFF_CM4_TO_CM7_MESSAGES_LEN MEM_ALIGN(sizeof(core_comm_channel))

/* Declaring pointers to shared memory structs */
extern const osMutexAttr_t comm_CM4_to_CM7_messages_mutex_attr;
extern volatile core_comm_channel * const comm_CM4_to_CM7_messages_ptr;

/* Initialization functions */
void core_comms_init_all_channels(void);
void core_comms_init_CM4_to_CM7_messages(void);

#endif /* CORE_COMMS_H */
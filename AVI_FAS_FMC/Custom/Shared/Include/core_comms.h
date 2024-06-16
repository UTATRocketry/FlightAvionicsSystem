#ifndef CORE_COMMS_H
#define CORE_COMMS_H

#include <stdint.h>
#include "cmsis_os2.h"

/* Communication channel properties */
#define CORE_COMM_CHANNEL_BUFFER_LEN (512)
#define CORE_COMM_MUTEX_WAIT (50)

typedef struct core_comm_channel {
    volatile uint8_t buffer[CORE_COMM_CHANNEL_BUFFER_LEN];
    osMutexId_t mutex_handle;
    volatile int ready_to_read;
    volatile int receiver_acknowledged;
} core_comm_channel;

/* Align x to 4 bytes */
#define MEM_ALIGN(x) (((x) + 0x00000003) & ~0x00000003)

/* Shared RAM space to be used is SRAM4 in D3 domain */
#define SHARED_RAM_ADDR (0x38000000)
#define SHARED_RAM_LEN  (0x0000FFFF)

/* Addresses of shared memory structs */
#define BUFF_CM4_TO_CM7_MESSAGES_ADDR MEM_ALIGN(SHARED_RAM_ADDR)
#define BUFF_CM4_TO_CM7_MESSAGES_LEN MEM_ALIGN(sizeof(core_comm_channel))

#define BUFF_CM7_TO_CM4_MESSAGES_ADDR MEM_ALIGN(SHARED_RAM_ADDR + BUFF_CM4_TO_CM7_MESSAGES_LEN)
#define BUFF_CM7_TO_CM4_MESSAGES_LEN MEM_ALIGN(sizeof(core_comm_channel))

/* 
    Utility function errors
    Ideally, this would be contained in a separate .h function called errors.h implemented by every other file
*/
#define CORE_COMM_ERROR_MUTEX_NOT_ACQUIRED (-1)
#define CORE_COMM_ERROR_MUTEX_NOT_RELEASED (-2)

/* Declaring pointers to shared memory structs */
extern const osMutexAttr_t comm_CM4_to_CM7_messages_mutex_attr;
extern volatile core_comm_channel * const comm_CM4_to_CM7_messages_ptr;

extern const osMutexAttr_t comm_CM7_to_CM4_messages_mutex_attr;
extern volatile core_comm_channel * const comm_CM7_to_CM4_messages_ptr;

/* Initialization functions */
void core_comms_init_all_channels(void); /* Calls subsequent init functions */
void core_comms_init_CM4_to_CM7_messages(void);
void core_comms_init_CM7_to_CM4_messages(void);

/* Sending and receiving functions */
int core_comms_channel_ready(volatile core_comm_channel* comm_ptr);
int core_comms_channel_acknowledged(volatile core_comm_channel* comm_ptr); 
int core_comms_channel_send(volatile core_comm_channel* comm_ptr, uint8_t* send_buffer, int buffer_len);
int core_comms_channel_receive(volatile core_comm_channel* comm_ptr, uint8_t* rcv_buffer, int buffer_len);

#endif /* CORE_COMMS_H */
#include <stdint.h>
#include <stdbool.h>

#define RTOS_WAIT_FOREVER 0xFFFFFFFF

// Returns current system ticks in milliseconds
extern uint32_t rtos_get_time_ms(void);

// RTOS delay (OS sleep, not a busy wait)
extern void rtos_delay_ms(uint32_t ms);

// Queue definition and send/receive function
typedef void* RtosQueueHandle_t;

extern bool rtos_queue_receive(RtosQueueHandle_t queue, void* buffer, uint32_t timeout_ms);

extern bool rtos_queue_send(RtosQueueHandle_t queue, void* data);

//Semaphore wait
extern void rtos_semaphore_wait(void* semaphore, uint32_t timeout_ms);

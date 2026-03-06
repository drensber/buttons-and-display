#include <stdint.h>
#include <stdbool.h>

#define RTOS_WAIT_FOREVER 0xFFFFFFFF

// Returns current system ticks in milliseconds
extern uint32_t rtos_get_time_ms(void);

// Queue definition and receive function
typedef void* RtosQueueHandle_t;

extern bool rtos_queue_receive(RtosQueueHandle_t queue, void* buffer, uint32_t timeout_ms);

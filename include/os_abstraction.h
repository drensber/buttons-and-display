#ifndef OS_ABSTRACTION_H
#define OS_ABSTRACTION_H

#include <stdint.h>
#include <stdbool.h>

#define RTOS_WAIT_FOREVER 0xFFFFFFFF

/*** Time and Timers ***/
// Timer primitives
typedef void* RtosTimerHandle_t;
typedef void (*RtosTimerCallback_t)(void);

// Need to be sure to trigger this callback from a timer
extern void minute_tick_callback(void);

// Returns current system ticks in milliseconds
extern uint32_t rtos_get_time_ms(void);

// RTOS delay (OS sleep, not a busy wait)
extern void rtos_delay_ms(uint32_t ms);


/*** Queues ***/
typedef void* RtosQueueHandle_t;

extern bool rtos_queue_receive(RtosQueueHandle_t queue,
			       void* buffer, uint32_t timeout_ms);
extern bool rtos_queue_send(RtosQueueHandle_t queue, void* data);


/*** Semaphores ***/
typedef void* RtosSemaphoreHandle_t;

extern void rtos_semaphore_wait(RtosSemaphoreHandle_t semaphore,
				uint32_t timeout_ms);


// Initializes all OS primitives (queues, semaphores, timers) and spawns tasks
extern void rtos_system_init(void);

#endif // OS_ABSTRACTION_H

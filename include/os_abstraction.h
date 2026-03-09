#ifndef OS_ABSTRACTION_H
#define OS_ABSTRACTION_H

#include <stdint.h>
#include <stdbool.h>

#define RTOS_WAIT_FOREVER 0xFFFFFFFF

/*** Time and Timers ***/
// Timer primitives
typedef void* RtosTimerHandle_t;
typedef void (*RtosTimerCallback_t)(void);

// Create a software timer
// period_ms: How often the timer fires
// is_periodic: true = auto-reloads forever, false = fires only once
extern RtosTimerHandle_t rtos_timer_create(
    const char* name, 
    uint32_t period_ms, 
    bool is_periodic, 
    RtosTimerCallback_t callback
);

// Start the timer
extern bool rtos_timer_start(RtosTimerHandle_t timer);

// Returns current system ticks in milliseconds
extern uint32_t rtos_get_time_ms(void);

// RTOS delay (OS sleep, not a busy wait)
extern void rtos_delay_ms(uint32_t ms);


/*** Queues ***/
typedef void* RtosQueueHandle_t;

extern bool rtos_initialize_queue(RtosQueueHandle_t queue);
extern bool rtos_queue_receive(RtosQueueHandle_t queue,
			       void* buffer, uint32_t timeout_ms);
extern bool rtos_queue_send(RtosQueueHandle_t queue, void* data);


/*** Semaphores ***/
typedef void* RtosSemaphoreHandle_t;

extern void rtos_semaphore_wait(RtosSemaphoreHandle_t semaphore,
				uint32_t timeout_ms);

/*** Threads ***/
// Define the signature for a task entry point
typedef void (*RtosTaskFunction_t)(void);

typedef enum {
    TASK_PRIORITY_LOWEST,
    TASK_PRIORITY_LOW,
    TASK_PRIORITY_MEDIUM,
    TASK_PRIORITY_HIGH,
    TASK_PRIORITY_HIGHEST
} RtosTaskPriority_t;

// Abstract thread creation
// Returns true on success, false on failure
extern bool rtos_thread_create(
    RtosTaskFunction_t task_func, 
    const char* task_name, 
    uint32_t stack_size, 
    RtosTaskPriority_t priority
);

// Optional to implement: A function to start the RTOS scheduler (if required by the
// underlying OS).
// Note: Zephyr starts its scheduler automatically before main(),
// but FreeRTOS requires this. OS that doesn't need this should still implement a NOP function.
extern void rtos_start_scheduler(void);

#endif // OS_ABSTRACTION_H

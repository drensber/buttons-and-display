#include <stdio.h>
#include "os_abstraction.h"

uint32_t rtos_get_time_ms(void)
{
    printf("Calling rtos_get_time_ms()\n");
    return 0;
}

bool rtos_initialize_queue(RtosQueueHandle_t queue)
{
    printf("Calling rtos_initialize_queue()\n");
    return true;
}

bool rtos_queue_receive(RtosQueueHandle_t queue, void* buffer, uint32_t timeout_ms)
{
    printf("Calling rtos_queue_receive()\n");
    return false;
}

bool rtos_queue_send(RtosQueueHandle_t queue, void* data)
{
    printf("Calling rtos_queue_send()\n");
    return true;
}

// Add mock implementations for the new primitives
void rtos_semaphore_wait(void* semaphore, uint32_t timeout_ms)
{
    printf("Calling rtos_semaphore_wait()\n");
}

void rtos_delay_ms(uint32_t ms)
{
    printf("Calling rtos_delay_ms(ms=%u)\n", ms);
}

bool rtos_thread_create(RtosTaskFunction_t task_func, 
			const char* task_name, 
			uint32_t stack_size, 
			RtosTaskPriority_t priority)
{
    printf("Calling rtos_thread_create()\n");
    return false;
}

void rtos_start_scheduler(void)
{
    printf("Calling rtos_start_scheduler() (in Zephyr, this ia a NOP)\n");
    return;
}

RtosTimerHandle_t rtos_timer_create(const char* name, uint32_t period_ms, bool is_periodic, RtosTimerCallback_t callback)
{
    printf("Creating %s timer '%s' (Period: %u ms)\n", 
           is_periodic ? "periodic" : "one-shot", name, period_ms);
    
    // Return a dummy non-null pointer for the mock
    return (RtosTimerHandle_t)1; 
}

bool rtos_timer_start(RtosTimerHandle_t timer)
{
    printf("Starting timer\n");
    return true;
}

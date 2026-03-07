#include <stdio.h>
#include "os_abstraction.h"

uint32_t rtos_get_time_ms(void)
{
    printf("Calling rtos_get_time_ms()\n");
    return 0;
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

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

bool rtos_queue_send(RtosQueueHandle_t queue, void* data, bool from_isr)
{
    printf("Calling rtos_queue_send(from_isr=%s)\n", from_isr ? "true" : "false");
    return true;
}

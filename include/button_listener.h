#ifndef BUTTON_LISTENER_H
#define BUTTON_LISTENER_H

#include "os_abstraction.h"

// Task entry point
extern void button_listener_task(RtosQueueHandle_t event_queue, void* isr_semaphore);

#endif // BUTTON_LISTENER_H

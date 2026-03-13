#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"

#include "os_abstraction.h"
#include "state_manager.h"
#include "display_manager.h"
#include "button_listener.h"

// Global handles expected by the application
RtosQueueHandle_t state_message_queue;
RtosQueueHandle_t display_message_queue;

// FreeRTOS specific handles
static QueueHandle_t f_state_msgq;
static QueueHandle_t f_display_msgq;
static TimerHandle_t f_sys_timer;

// Wrapper for the FreeRTOS timer callback
static void freertos_timer_callback(TimerHandle_t xTimer)
{
    // Call the generic application callback
    minute_tick_callback();
}

void rtos_system_init(void)
{
    printf("OSAL: Initializing FreeRTOS primitives...\n");

    // Create Queues
    f_state_msgq = xQueueCreate(10, sizeof(StateMsg_t));
    f_display_msgq = xQueueCreate(10, sizeof(DisplayMsg_t));

    // Assign them to the global abstraction handles
    state_message_queue = f_state_msgq;
    display_message_queue = f_display_msgq;

    // Create Tasks (Function, Name, Stack Size, Params, Priority, Handle)
    // Note: ESP-IDF FreeRTOS stack size is in bytes. 4096 is safe.
    xTaskCreate((TaskFunction_t)display_manager_task, "display_task", 4096, NULL, 5, NULL);
    xTaskCreate((TaskFunction_t)button_listener_task, "button_task",  4096, NULL, 5, NULL);
    xTaskCreate((TaskFunction_t)state_manager_task,   "state_task",   4096, NULL, 5, NULL);

    // 3. Create and Start System Timer (60000 ms period, auto-reload pdTRUE)
    f_sys_timer = xTimerCreate("sys_timer",
			       pdMS_TO_TICKS(60000),
			       pdTRUE, (void *)0,
			       freertos_timer_callback);
    xTimerStart(f_sys_timer, 0);

    printf("OSAL: FreeRTOS primitives initialized.\n");
}

uint32_t rtos_get_time_ms(void) {
    return (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS);
}

void rtos_delay_ms(uint32_t ms) {
    vTaskDelay(pdMS_TO_TICKS(ms));
}

bool rtos_queue_receive(RtosQueueHandle_t queue, void* buffer, uint32_t timeout_ms) {
    TickType_t ticks = ((timeout_ms == RTOS_WAIT_FOREVER) ?
			portMAX_DELAY :
			pdMS_TO_TICKS(timeout_ms));
    return (xQueueReceive((QueueHandle_t)queue, buffer, ticks) == pdTRUE);
}

bool rtos_queue_send(RtosQueueHandle_t queue, void* data) {
    // Send with 0 timeout for non-blocking (K_NO_WAIT equivalent)
    return (xQueueSend((QueueHandle_t)queue, data, 0) == pdTRUE);
}

void rtos_semaphore_wait(RtosSemaphoreHandle_t semaphore, uint32_t timeout_ms) {
    TickType_t ticks = ((timeout_ms == RTOS_WAIT_FOREVER) ?
			portMAX_DELAY :
			pdMS_TO_TICKS(timeout_ms));
    xSemaphoreTake((SemaphoreHandle_t)semaphore, ticks);
}

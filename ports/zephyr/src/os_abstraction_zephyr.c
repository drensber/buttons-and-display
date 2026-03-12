#include <stdio.h>
#include <zephyr/kernel.h>
#include "os_abstraction.h"
#include "state_manager.h"
#include "display_manager.h"
#include "button_listener.h"

/* ========================================================================= *
 * Static Allocations (Zephyr Macros)
 * ========================================================================= */

// 1. Queues
K_MSGQ_DEFINE(z_state_msgq, sizeof(StateMsg_t), 10, 4);
K_MSGQ_DEFINE(z_display_msgq, sizeof(DisplayMsg_t), 10, 4);

// Map the static Zephyr objects to the abstract global handles expected by the app
RtosQueueHandle_t state_message_queue = &z_state_msgq;

RtosQueueHandle_t display_message_queue = &z_display_msgq;

// 2. Threads (Auto-started by Zephyr before main)
// K_THREAD_DEFINE(name, stack_size, entry_fn, p1, p2, p3, prio, options, delay)
K_THREAD_DEFINE(display_tid, 1024, display_manager_task, NULL, NULL, NULL, 5, 0, 0);
K_THREAD_DEFINE(button_tid,  1024, button_listener_task, NULL, NULL, NULL, 5, 0, 0);
K_THREAD_DEFINE(state_tid,   1024, state_manager_task,   NULL, NULL, NULL, 5, 0, 0);

// 3. System Timer
extern void minute_tick_callback(void); // Declared in main.c
K_TIMER_DEFINE(z_sys_timer, (k_timer_expiry_t)minute_tick_callback, NULL);


/* ========================================================================= *
 * Initialization and Scheduler
 * ========================================================================= */

void rtos_system_init(void)
{
    printf("OSAL: Starting system timer...\n");
    // Start the static timer (60 seconds duration, 60 seconds period)
    k_timer_start(&z_sys_timer, K_MSEC(60000), K_MSEC(60000));

    k_sleep(K_FOREVER);
}

/* ========================================================================= *
 * OS API Implementations
 * ========================================================================= */

uint32_t rtos_get_time_ms(void) {
    return k_uptime_get_32();
}

void rtos_delay_ms(uint32_t ms) {
    k_msleep(ms);
}

bool rtos_queue_receive(RtosQueueHandle_t queue, void* buffer, uint32_t timeout_ms) {
    struct k_msgq *q = (struct k_msgq *)queue;
    k_timeout_t z_timeout = (timeout_ms == RTOS_WAIT_FOREVER) ? K_FOREVER : K_MSEC(timeout_ms);
    return (k_msgq_get(q, buffer, z_timeout) == 0);
}

bool rtos_queue_send(RtosQueueHandle_t queue, void* data) {
    struct k_msgq *q = (struct k_msgq *)queue;
    return (k_msgq_put(q, data, K_NO_WAIT) == 0);
}

void rtos_semaphore_wait(RtosSemaphoreHandle_t semaphore, uint32_t timeout_ms) {
    struct k_sem *sem = (struct k_sem *)semaphore;
    k_timeout_t z_timeout = (timeout_ms == RTOS_WAIT_FOREVER) ? K_FOREVER : K_MSEC(timeout_ms);
    k_sem_take(sem, z_timeout);
}

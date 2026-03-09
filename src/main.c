/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include "state_manager.h"
#include "display_manager.h"
#include "button_listener.h"
#include "os_abstraction.h"

// The function the OS will call every time the timer expires
void system_tick_callback(void)
{
    StateMsg_t msg;
    msg.event = EVENT_SYS_TICK_MINUTE;
    
    // Drop the event into the queue.
    // (In a real Zephyr/FreeRTOS implementation behind your OSAL, this 
    // underlying queue send needs to have a timeout of 0 / K_NO_WAIT 
    // so it doesn't block the OS timer daemon!)
    rtos_queue_send(state_message_queue, &msg);
}

int main(void)
{
    //Iniitalize message queues here
    printf("Initializing queues.\n");
    rtos_initialize_queue(state_message_queue);
    rtos_initialize_queue(display_message_queue);
    
    // Create and initialize tasks
    printf("Starting tasks.\n");
    rtos_thread_create(display_manager_task, "DisplayManagerTask",
		       1024, TASK_PRIORITY_MEDIUM);
    rtos_thread_create(button_listener_task, "ButtonListenerTask",
		       1024, TASK_PRIORITY_MEDIUM);
    rtos_thread_create(state_manager_task, "StateManagerTask",
		       1024, TASK_PRIORITY_MEDIUM);

    rtos_start_scheduler();

    printf("Setting \"minute change\" timer...\n");    
    // Create a timer that fires every 60,000 ms (1 minute) and repeats (true)
    RtosTimerHandle_t minute_timer = rtos_timer_create(
        "SysTickTimer", 
        60000, 
        true, 
        system_tick_callback
    );
    
    // Start the timer ticking
    rtos_timer_start(minute_timer);    
    
        
    return 0;
}

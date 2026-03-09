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

int main(void)
{
    //Iniitalize message queues here
    
    printk("Starting tasks!\n");
    rtos_thread_create(display_manager_task, "DisplayManagerTask",
		       1024, TASK_PRIORITY_MEDIUM);
    rtos_thread_create(button_listener_task, "ButtonListenerTask",
		       1024, TASK_PRIORITY_MEDIUM);
    rtos_thread_create(state_manager_task, "StateManagerTask",
		       1024, TASK_PRIORITY_MEDIUM);

    rtos_start_scheduler();
        
    return 0;
}

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
    RtosQueueHandle_t display_queue = NULL;
    RtosQueueHandle_t event_queue = NULL;    
    
    printk("Starting tasks!\n");
    display_task(display_queue);
    state_manager_task(event_queue, display_queue);
    button_listener_task(event_queue, NULL);
        
    return 0;
}

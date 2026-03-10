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
    printf("Initializing OS primitives and tasks...\n");
    
    // Wires up all queues, threads, and timers
    // Then blocks forever.    
    rtos_system_init();
        
    return 0;
}

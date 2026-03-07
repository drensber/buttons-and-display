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
    display_manager_task();
    state_manager_task();
    button_listener_task();
        
    return 0;
}

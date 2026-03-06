/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include "display_manager.h"
#include "os_abstraction.h"

int main(void)
{
    RtosQueueHandle_t handle = NULL;
    
    printk("Starting display_manager!\n");
    display_task(handle);
    
    return 0;
}

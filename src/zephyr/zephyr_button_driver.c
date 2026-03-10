#include <stdio.h>
#include <zephyr/kernel.h>
#include "hw_abstraction.h"


// Statically define the semaphore (initial count 0, max count 1)
K_SEM_DEFINE(z_btn_sem, 0, 1);

RtosSemaphoreHandle_t hw_btn_semaphore_setup(void)
{
    // Just return the pointer to the statically allocated semaphore
    return &z_btn_sem;
}

bool hw_btn_read_alarm(void)
{
    // Mock implementation
    return false;
}

bool hw_btn_read_digit(void)
{
    // Mock implementation
    return false; 
}

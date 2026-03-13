#include <stdio.h>
#include "os_abstraction.h"

void app_main(void)
{
    printf("ESP-IDF: Initializing OS primitives and tasks...\n");
    rtos_system_init();
}

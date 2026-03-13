#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "hw_abstraction.h"

#define ALARM_BTN_GPIO 4
#define DIGIT_BTN_GPIO 5

static SemaphoreHandle_t f_btn_sem = NULL;

// ESP32 Hardware Interrupt Service Routine
static void IRAM_ATTR button_isr_handler(void* arg) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(f_btn_sem, &xHigherPriorityTaskWoken);
    
    // Force a context switch if giving the semaphore woke a higher priority task
    if (xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR();
    }
}

RtosSemaphoreHandle_t hw_btn_semaphore_setup(void) {
    // Create a binary semaphore for the ISR to give
    f_btn_sem = xSemaphoreCreateBinary();

    // Configure the GPIO pins (Inputs, Pull-ups enabled, trigger on ANY edge)
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_ANYEDGE,
        .pin_bit_mask = (1ULL << ALARM_BTN_GPIO) | (1ULL << DIGIT_BTN_GPIO),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE
    };
    gpio_config(&io_conf);

    // Install the global ISR service and attach our specific handlers
    gpio_install_isr_service(0);
    gpio_isr_handler_add(ALARM_BTN_GPIO, button_isr_handler, NULL);
    gpio_isr_handler_add(DIGIT_BTN_GPIO, button_isr_handler, NULL);

    printf("ESP32: Hardware Button GPIOs and ISRs successfully configured.\n");
    
    return (RtosSemaphoreHandle_t)f_btn_sem;
}

bool hw_btn_read_alarm(void) {
    // Active low: returns true if the pin is pulled to ground (pressed)
    return gpio_get_level(ALARM_BTN_GPIO) == 0;
}

bool hw_btn_read_digit(void) {
    return gpio_get_level(DIGIT_BTN_GPIO) == 0;
}

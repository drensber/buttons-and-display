#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include "hw_abstraction.h"

// Statically define the semaphore (initial count 0, max count 1)
K_SEM_DEFINE(z_btn_sem, 0, 1);

// Fetch the hardware definitions from the app.overlay
static const struct gpio_dt_spec alarm_btn = GPIO_DT_SPEC_GET(DT_ALIAS(alarm_button), gpios);
static const struct gpio_dt_spec digit_btn = GPIO_DT_SPEC_GET(DT_ALIAS(digit_button), gpios);

// A struct to hold the callback bindings for Zephyr
static struct gpio_callback button_cb_data;

// The Hardware Interrupt Service Routine
static void button_pressed_isr(const struct device *port, struct gpio_callback *cb, uint32_t pins)
{
    // We are in an ISR! Do the bare minimum: wake up the listener task.
    k_sem_give(&z_btn_sem);
}

RtosSemaphoreHandle_t hw_btn_semaphore_setup(void)
{
    // 1. Verify the hardware defined in the device tree is ready
    if (!gpio_is_ready_dt(&alarm_btn) || !gpio_is_ready_dt(&digit_btn)) {
        printf("Error: Button GPIO devices not ready\n");
        return NULL;
    }

    // 2. Configure the pins as inputs (the dt_spec automatically applies the pull-ups)
    gpio_pin_configure_dt(&alarm_btn, GPIO_INPUT);
    gpio_pin_configure_dt(&digit_btn, GPIO_INPUT);

    // 3. Configure the interrupts to fire on BOTH edges (press and release)
    gpio_pin_interrupt_configure_dt(&alarm_btn, GPIO_INT_EDGE_BOTH);
    gpio_pin_interrupt_configure_dt(&digit_btn, GPIO_INT_EDGE_BOTH);

    // 4. Register the shared callback for both pins
    gpio_init_callback(&button_cb_data, button_pressed_isr, BIT(alarm_btn.pin) | BIT(digit_btn.pin));
    
    // Attach the callback to the specific GPIO ports
    gpio_add_callback(alarm_btn.port, &button_cb_data);
    gpio_add_callback(digit_btn.port, &button_cb_data);

    printf("Hardware Button GPIOs and ISRs successfully configured.\n");
    
    return &z_btn_sem;
}

bool hw_btn_read_alarm(void)
{
    // Returns 1 if active (pressed, because of GPIO_ACTIVE_LOW in overlay), 0 if inactive
    return gpio_pin_get_dt(&alarm_btn) == 1;
}

bool hw_btn_read_digit(void)
{
    return gpio_pin_get_dt(&digit_btn) == 1;
}

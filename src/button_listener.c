#include "button_listener.h"
#include "state_manager.h"
#include "hw_abstraction.h"
#include <stddef.h>

void button_listener_task(RtosQueueHandle_t event_queue, void* isr_semaphore) {
    bool last_alarm_state = false;
    bool last_digit_state = false;

    while (1) {
        // 1. Sleep entirely until the actual hardware ISR gives this semaphore
        rtos_semaphore_wait(isr_semaphore, RTOS_WAIT_FOREVER);

        // 2. Debounce: Wait 20ms for the physical switch contacts to settle
        rtos_delay_ms(20);

        // 3. Read the stable hardware states
        bool current_alarm_state = hw_btn_read_alarm();
        bool current_digit_state = hw_btn_read_digit();

        // 4. Compare to previous state and emit events ONLY on state changes
        if (current_alarm_state != last_alarm_state) {
            StateEvent_t ev = current_alarm_state ? EV_BTN_ALARM_PRESSED : EV_BTN_ALARM_RELEASED;
            rtos_queue_send(event_queue, &ev);
            last_alarm_state = current_alarm_state;
        }

        if (current_digit_state != last_digit_state) {
            StateEvent_t ev = current_digit_state ? EV_BTN_DIGIT_PRESSED : EV_BTN_DIGIT_RELEASED;
            rtos_queue_send(event_queue, &ev);
            last_digit_state = current_digit_state;
        }
    }
}

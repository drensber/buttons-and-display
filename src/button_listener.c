#include <stddef.h>
#include <stdio.h>
#include "button_listener.h"
#include "state_manager.h"
#include "hw_abstraction.h"

void button_listener_task(void)
{
    StateMsg_t msg;
    
    bool last_alarm_state = false;
    bool last_digit_state = false;

    RtosSemaphoreHandle_t hw_btn_isr_semaphore
	= hw_btn_semaphore_setup();
   
    if (hw_btn_isr_semaphore == NULL) {
	printf("Problem setting up button semaphore\n");
	return;
    }

    while (1) {
        // 1. Sleep entirely until the actual hardware ISR gives this semaphore
        rtos_semaphore_wait(hw_btn_isr_semaphore, RTOS_WAIT_FOREVER);

        // 2. Debounce: Wait 20ms for the physical switch contacts to settle
        rtos_delay_ms(20);

        // 3. Read the stable hardware states
        bool current_alarm_state = hw_btn_read_alarm();
        bool current_digit_state = hw_btn_read_digit();

        // 4. Compare to previous state and emit events ONLY on state changes
        if (current_alarm_state != last_alarm_state) {
            msg.event = current_alarm_state ? EVENT_BUTTON_ALARM_PRESSED : EVENT_BUTTON_ALARM_RELEASED;
            rtos_queue_send(state_message_queue, &msg);
            last_alarm_state = current_alarm_state;
        }

        if (current_digit_state != last_digit_state) {
            msg.event = current_digit_state ? EVENT_BUTTON_DIGIT_PRESSED : EVENT_BUTTON_DIGIT_RELEASED;
            rtos_queue_send(state_message_queue, &msg);
            last_digit_state = current_digit_state;
        }
    }
    return;
}

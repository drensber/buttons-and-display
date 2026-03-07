#include "state_manager.h"
#include "display_manager.h"

// Posts an event to the state manager's queue. Called by hardware interrupts/timers.
bool state_manager_post_event(RtosQueueHandle_t event_queue, StateEvent_t event, bool from_isr) {
    return rtos_queue_send(event_queue, &event, from_isr);
}

void state_manager_task(RtosQueueHandle_t event_queue, RtosQueueHandle_t display_queue) {
    StateEvent_t event;
    
    // Internal state tracking
    bool btn_alarm_active = false;
    bool btn_digit_active = false;
    DisplayViewState_t current_view = VIEW_STATE_TIME;

    // Dummy system data
    uint8_t sys_hours = 12, sys_minutes = 0;
    uint8_t alarm_hours = 6, alarm_minutes = 30;
    bool alarm_enabled = true;
    uint8_t display_digit = 8; 

    // Initial push to the display
    DisplayMsg_t init_msg;
    init_msg.type = MSG_UPDATE_TIME;
    init_msg.payload.time.hours = sys_hours;
    init_msg.payload.time.minutes = sys_minutes;
    rtos_queue_send(display_queue, &init_msg, false);

    init_msg.type = MSG_UPDATE_ALARM_STATE;
    init_msg.payload.alarm.is_set = alarm_enabled;
    init_msg.payload.alarm.hours = alarm_hours;
    init_msg.payload.alarm.minutes = alarm_minutes;
    rtos_queue_send(display_queue, &init_msg, false);

    while (1) {
        // Wait for an event from the button listener or system timer
        if (rtos_queue_receive(event_queue, &event, RTOS_WAIT_FOREVER)) {
            
            bool evaluate_view = false;

            // 1. Process the incoming event
            switch (event) {
                case EV_BTN_ALARM_PRESSED:
                    btn_alarm_active = true;
                    evaluate_view = true;
                    break;
                case EV_BTN_ALARM_RELEASED:
                    btn_alarm_active = false;
                    evaluate_view = true;
                    break;
                case EV_BTN_DIGIT_PRESSED:
                    btn_digit_active = true;
                    evaluate_view = true;
                    break;
                case EV_BTN_DIGIT_RELEASED:
                    btn_digit_active = false;
                    evaluate_view = true;
                    break;
                case EV_SYS_TICK_MINUTE:
                    sys_minutes++;
                    if (sys_minutes >= 60) {
			sys_minutes = 0;
			sys_hours = (sys_hours + 1) % 24;
		    }
                    
                    DisplayMsg_t time_msg = { .type = MSG_UPDATE_TIME };
                    time_msg.payload.time.hours = sys_hours;
                    time_msg.payload.time.minutes = sys_minutes;
                    rtos_queue_send(display_queue, &time_msg, false);
                    break;
            }

            // 2. Resolve View Priority
            if (evaluate_view) {
                DisplayViewState_t target_view = VIEW_STATE_TIME;

                // Priority Logic: Digit overrides Alarm
                if (btn_digit_active) {
                    target_view = VIEW_STATE_DIGIT;
                } else if (btn_alarm_active) {
                    target_view = VIEW_STATE_ALARM;
                }

                // 3. Command the Display (only if state changed)
                if (target_view != current_view) {
                    current_view = target_view;
                    
                    DisplayMsg_t view_msg = { .type = MSG_SET_ACTIVE_VIEW };
                    view_msg.payload.view_ctrl.view = current_view;
                    view_msg.payload.view_ctrl.digit_val = display_digit;
                    
                    rtos_queue_send(display_queue, &view_msg, false);
                }
            }
        }
    }
}

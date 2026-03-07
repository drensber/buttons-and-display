#include "state_manager.h"
#include "display_manager.h"

static struct {
    bool btn_alarm_active;
    bool btn_digit_active;
    DisplayViewState_t current_view;
    
    uint8_t sys_hours;
    uint8_t sys_minutes;
    uint8_t alarm_hours;
    uint8_t alarm_minutes;
    bool alarm_enabled;
    uint8_t display_digit; 
} sm_ctx;

static void initialize_state_manager(void) {
    // Internal state tracking
    sm_ctx.btn_alarm_active = false;
    sm_ctx.btn_digit_active = false;
    sm_ctx.current_view = VIEW_STATE_TIME;

    // Dummy system data
    sm_ctx.sys_hours = 12;
    sm_ctx.sys_minutes = 0;
    sm_ctx.alarm_hours = 6;
    sm_ctx.alarm_minutes = 30;
    sm_ctx.alarm_enabled = true;
    sm_ctx.display_digit = 8; 
}

static void initialize_display(RtosQueueHandle_t queue) {
    // Initial push to the display
    DisplayMsg_t init_msg;
    init_msg.type = MSG_UPDATE_TIME;
    init_msg.payload.time.hours = sm_ctx.sys_hours;
    init_msg.payload.time.minutes = sm_ctx.sys_minutes;
    rtos_queue_send(queue, &init_msg);

    init_msg.type = MSG_UPDATE_ALARM_STATE;
    init_msg.payload.alarm.is_set = sm_ctx.alarm_enabled;
    init_msg.payload.alarm.hours = sm_ctx.alarm_hours;
    init_msg.payload.alarm.minutes = sm_ctx.alarm_minutes;
    rtos_queue_send(queue, &init_msg);


}

void state_manager_task(RtosQueueHandle_t event_queue,
			RtosQueueHandle_t display_queue) {
    StateEvent_t event;

    initialize_state_manager();

    initialize_display(display_queue);

    while (1) {
        // Wait for an event from the button listener or system timer
        if (rtos_queue_receive(event_queue, &event, RTOS_WAIT_FOREVER)) {
            
            bool evaluate_view = false;

            // 1. Process the incoming event
            switch (event) {
                case EV_BTN_ALARM_PRESSED:
                    sm_ctx.btn_alarm_active = true;
                    evaluate_view = true;
                    break;
                case EV_BTN_ALARM_RELEASED:
                    sm_ctx.btn_alarm_active = false;
                    evaluate_view = true;
                    break;
                case EV_BTN_DIGIT_PRESSED:
                    sm_ctx.btn_digit_active = true;
                    evaluate_view = true;
                    break;
                case EV_BTN_DIGIT_RELEASED:
                    sm_ctx.btn_digit_active = false;
                    evaluate_view = true;
                    break;
                case EV_SYS_TICK_MINUTE:
                    sm_ctx.sys_minutes++;
                    if (sm_ctx.sys_minutes >= 60) {
			sm_ctx.sys_minutes = 0;
			sm_ctx.sys_hours = (sm_ctx.sys_hours + 1) % 24;
		    }                    
                    DisplayMsg_t time_msg = { .type = MSG_UPDATE_TIME };
                    time_msg.payload.time.hours = sm_ctx.sys_hours;
                    time_msg.payload.time.minutes = sm_ctx.sys_minutes;
                    rtos_queue_send(display_queue, &time_msg);
                    break;
            }

            // 2. Resolve View Priority
            if (evaluate_view) {
                DisplayViewState_t target_view = VIEW_STATE_TIME;

                // Priority Logic: Digit overrides Alarm
                if (sm_ctx.btn_digit_active) {
                    target_view = VIEW_STATE_DIGIT;
                } else if (sm_ctx.btn_alarm_active) {
                    target_view = VIEW_STATE_ALARM;
                }

                // 3. Command the Display (only if state changed)
                if (target_view != sm_ctx.current_view) {
                    sm_ctx.current_view = target_view;
                    
                    DisplayMsg_t view_msg = { .type = MSG_SET_ACTIVE_VIEW };
                    view_msg.payload.view_ctrl.view = sm_ctx.current_view;
                    view_msg.payload.view_ctrl.digit_val = sm_ctx.display_digit;
                    
                    rtos_queue_send(display_queue, &view_msg);
                }
            }
        }
    }
}

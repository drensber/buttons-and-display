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

static void initialize_sm_ctx(void)
{
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

static void initialize_display(RtosQueueHandle_t queue)
{
    // Initial push to the display
    DisplayMsg_t init_msg;
    init_msg.event = DISPLAY_EVENT_UPDATE_TIME;
    init_msg.payload.time.hours = sm_ctx.sys_hours;
    init_msg.payload.time.minutes = sm_ctx.sys_minutes;
    rtos_queue_send(queue, &init_msg);

    init_msg.event = DISPLAY_EVENT_UPDATE_ALARM_STATE;
    init_msg.payload.alarm.is_set = sm_ctx.alarm_enabled;
    init_msg.payload.alarm.hours = sm_ctx.alarm_hours;
    init_msg.payload.alarm.minutes = sm_ctx.alarm_minutes;
    rtos_queue_send(queue, &init_msg);


}

// The function the OS will call every time the timer expires
void minute_tick_callback(void)
{
    StateMsg_t msg;
    msg.event = EVENT_SYS_TICK_MINUTE;
    
    // Drop the event into the queue.
    // (In a real Zephyr/FreeRTOS implementation behind your OSAL, this 
    // underlying queue send needs to have a timeout of 0 / K_NO_WAIT 
    // so it doesn't block the OS timer daemon!)
    rtos_queue_send(state_message_queue, &msg);
}

void state_manager_process_event(StateMsg_t *msg)
{
    DisplayViewState_t target_view = sm_ctx.current_view;

    // 1. Process the incoming event
    switch (msg->event) {
        case EVENT_BUTTON_ALARM_PRESSED:
            sm_ctx.btn_alarm_active = true;
	    target_view = VIEW_STATE_ALARM;
            break;
        case EVENT_BUTTON_ALARM_RELEASED:
            sm_ctx.btn_alarm_active = false;
	    if (target_view == VIEW_STATE_ALARM) {
		target_view = (sm_ctx.btn_digit_active ?
			       VIEW_STATE_DIGIT :
			       VIEW_STATE_TIME);
	    }
            break;
        case EVENT_BUTTON_DIGIT_PRESSED:
            sm_ctx.btn_digit_active = true;
	    target_view = VIEW_STATE_DIGIT;
            break;
        case EVENT_BUTTON_DIGIT_RELEASED:
            sm_ctx.btn_digit_active = false;
	    if (target_view == VIEW_STATE_DIGIT) {
		target_view = (sm_ctx.btn_alarm_active ?
			       VIEW_STATE_ALARM :
			       VIEW_STATE_TIME);
	    }
            break;
        case EVENT_SYS_TICK_MINUTE:
            sm_ctx.sys_minutes++;
            if (sm_ctx.sys_minutes >= 60) {
                sm_ctx.sys_minutes = 0;
                sm_ctx.sys_hours = (sm_ctx.sys_hours + 1) % 24;
            }                    
            DisplayMsg_t time_msg = { .event = DISPLAY_EVENT_UPDATE_TIME };
            time_msg.payload.time.hours = sm_ctx.sys_hours;
            time_msg.payload.time.minutes = sm_ctx.sys_minutes;
            rtos_queue_send(display_message_queue, &time_msg);
            break;
    }
    if (target_view != sm_ctx.current_view) {
	sm_ctx.current_view = target_view;
	DisplayMsg_t view_msg = { .event = DISPLAY_EVENT_SET_ACTIVE_VIEW };
	view_msg.payload.view_ctrl.view = target_view;
	view_msg.payload.view_ctrl.digit_val = sm_ctx.display_digit;
	rtos_queue_send(display_message_queue, &view_msg);
    }
}

void initialize_state_manager(void)
{
    initialize_sm_ctx();

    initialize_display(display_message_queue);
}

void state_manager_task(void)
{
    StateMsg_t msg;

    initialize_state_manager();
    
    while (1) {
        // Wait for an event from the button listener or system timer
        if (rtos_queue_receive(state_message_queue, &msg, RTOS_WAIT_FOREVER)) {
	    state_manager_process_event(&msg);
        }
    }
}

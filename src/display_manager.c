#include <stdio.h>
#include <string.h>
#include "display_manager.h"
#include "os_abstraction.h"
#include "hardware_abstraction_display.h"

static struct {
    DisplayViewState_t current_view;
    
    uint8_t current_hours;
    uint8_t current_minutes;
    
    bool alarm_is_set;
    uint8_t alarm_hours;
    uint8_t alarm_minutes;
    
    uint8_t temp_digit;
} display_manager_context;


// Helper function to render the current state to the hardware
static void render_display(void) {
    char render_buffer[8]; // Enough to hold "HH:MM" + null terminator
    
    disp_hw_clear();

    switch (display_manager_context.current_view) {
        case VIEW_STATE_TIME:
            snprintf(render_buffer, sizeof(render_buffer), "%02d:%02d",
		     display_manager_context.current_hours, display_manager_context.current_minutes);
            disp_hw_write_text(render_buffer);
            break;

        case VIEW_STATE_ALARM:
            if (display_manager_context.alarm_is_set) {
                snprintf(render_buffer, sizeof(render_buffer), "%02d:%02d",
			 display_manager_context.alarm_hours, display_manager_context.alarm_minutes);
            } else {
                snprintf(render_buffer, sizeof(render_buffer), "--:--");
            }
            disp_hw_write_text(render_buffer);
            break;

        case VIEW_STATE_DIGIT:
            snprintf(render_buffer, sizeof(render_buffer), "%d", display_manager_context.temp_digit);
            disp_hw_write_text(render_buffer);
            break;
    }

    // The alarm indicator dot is persistently rendered regardless of the current view
    disp_hw_set_upper_right_dot(display_manager_context.alarm_is_set);
}

/* ========================================================================= *
 * Display Task - Main RTOS Entry Point                                      *
 * ========================================================================= */

void display_task(RtosQueueHandle_t msg_queue) {
    DisplayMsg_t msg;

    // Initialize state
    memset(&display_manager_context, 0, sizeof(display_manager_context));
    display_manager_context.current_view = VIEW_STATE_TIME;
    render_display();

    while (1) {
        // Wait indefinitely for a message.
        if (rtos_queue_receive(msg_queue, &msg, RTOS_WAIT_FOREVER)) {
            
            // Process incoming message from higher-level state manager
            switch (msg.type) {
                case MSG_UPDATE_TIME:
                    display_manager_context.current_hours = msg.payload.time.hours;
                    display_manager_context.current_minutes = msg.payload.time.minutes;
                    break;

                case MSG_UPDATE_ALARM_STATE:
                    display_manager_context.alarm_is_set = msg.payload.alarm.is_set;
                    display_manager_context.alarm_hours = msg.payload.alarm.hours;
                    display_manager_context.alarm_minutes = msg.payload.alarm.minutes;
                    break;

                case MSG_SET_ACTIVE_VIEW:
                    display_manager_context.current_view = msg.payload.view_ctrl.view;
                    display_manager_context.temp_digit = msg.payload.view_ctrl.digit_val;
                    break;
            }

            render_display();
        }
    }
}

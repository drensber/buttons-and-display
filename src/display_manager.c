#include <stdio.h>
#include <string.h>
#include "display_manager.h"
#include "os_abstraction.h"
#include "hardware_abstraction_display.h"

static struct {
    DisplayViewState_t current_view;
    uint32_t view_timeout_ms;
    
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
        case VIEW_STATE_DEFAULT_TIME:
            snprintf(render_buffer, sizeof(render_buffer), "%02d:%02d",
		     display_manager_context.current_hours, display_manager_context.current_minutes);
            disp_hw_write_text(render_buffer);
            break;

        case VIEW_STATE_TEMP_ALARM:
            if (display_manager_context.alarm_is_set) {
                snprintf(render_buffer, sizeof(render_buffer), "%02d:%02d",
			 display_manager_context.alarm_hours, display_manager_context.alarm_minutes);
            } else {
                snprintf(render_buffer, sizeof(render_buffer), "--:--");
            }
            disp_hw_write_text(render_buffer);
            break;

        case VIEW_STATE_TEMP_DIGIT:
            snprintf(render_buffer, sizeof(render_buffer), "%d", display_manager_context.temp_digit);
            disp_hw_write_text(render_buffer);
            break;
    }

    // The alarm indicator dot is persistently rendered regardless of the current view
    disp_hw_set_upper_right_dot(display_manager_context.alarm_is_set);
}

// Helper to switch to a temporary view and set the timeout
static void set_temporary_view(DisplayViewState_t new_view) {
    display_manager_context.current_view = new_view;
    display_manager_context.view_timeout_ms = rtos_get_time_ms() + TEMP_DISPLAY_DURATION_MS;
    render_display();
}

/* ========================================================================= *
 * Display Task - Main RTOS Entry Point                                      *
 * ========================================================================= */

void display_task(RtosQueueHandle_t msg_queue) {
    DisplayMsg_t msg;
    uint32_t wait_time = RTOS_WAIT_FOREVER;

    // Initialize state
    memset(&display_manager_context, 0, sizeof(display_manager_context));
    display_manager_context.current_view = VIEW_STATE_DEFAULT_TIME;
    render_display();

    while (1) {
        // Wait for a message. If in a temporary view, wait only until the timeout expires.
        if (rtos_queue_receive(msg_queue, &msg, wait_time)) {
            
            // Process incoming message from higher-level state manager
            switch (msg.type) {
                case MSG_UPDATE_TIME:
                    display_manager_context.current_hours = msg.payload.time.hours;
                    display_manager_context.current_minutes = msg.payload.time.minutes;
                    if (display_manager_context.current_view == VIEW_STATE_DEFAULT_TIME) {
                        render_display();
                    }
                    break;

                case MSG_UPDATE_ALARM_STATE:
                    display_manager_context.alarm_is_set = msg.payload.alarm.is_set;
                    display_manager_context.alarm_hours = msg.payload.alarm.hours;
                    display_manager_context.alarm_minutes = msg.payload.alarm.minutes;
                    render_display(); // Rerender to instantly update the alarm dot
                    break;

                case MSG_CMD_SHOW_ALARM:
                    set_temporary_view(VIEW_STATE_TEMP_ALARM);
                    break;

                case MSG_CMD_SHOW_DIGIT:
                    display_manager_context.temp_digit = msg.payload.digit;
                    set_temporary_view(VIEW_STATE_TEMP_DIGIT);
                    break;
            }
        }

        // Check for temporary view timeouts
        if (display_manager_context.current_view != VIEW_STATE_DEFAULT_TIME) {
            uint32_t now = rtos_get_time_ms();
            if (now >= display_manager_context.view_timeout_ms) {
                // Timeout reached, revert to default time view
                display_manager_context.current_view = VIEW_STATE_DEFAULT_TIME;
                render_display();
                wait_time = RTOS_WAIT_FOREVER; // Go back to sleeping indefinitely
            } else {
                // Adjust wait time to wake up exactly when the view expires
                wait_time = display_manager_context.view_timeout_ms - now;
            }
        } else {
            wait_time = RTOS_WAIT_FOREVER;
        }
    }
}

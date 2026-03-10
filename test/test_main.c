#include <stdio.h>
#include <assert.h>
#include "os_abstraction.h"
#include "state_manager.h"
#include "display_manager.h"

// A helper to flush any messages currently sitting in the display queue
static void clear_display_queue(void) {
    DisplayMsg_t dummy;
    while (rtos_queue_receive(display_message_queue, &dummy, 0)) {
        // Just empty the queue
    }
}

void test_alarm_button_overrides_view(void) {
    printf("Running test: Alarm Button Overrides View...\n");
    
    // 1. Setup
    clear_display_queue();
    
    // 2. Synthesize an event (User presses the alarm button)
    StateMsg_t input_msg;
    input_msg.event = EVENT_BUTTON_ALARM_PRESSED;
    
    // 3. Execute the core logic
    state_manager_process_event(&input_msg);
    
    // 4. Verify the output
    DisplayMsg_t output_msg;
    bool received = rtos_queue_receive(display_message_queue, &output_msg, 0);
    
    // We expect the state manager to have commanded a view change
    assert(received == true);
    assert(output_msg.event == DISPLAY_EVENT_SET_ACTIVE_VIEW);
    assert(output_msg.payload.view_ctrl.view == VIEW_STATE_ALARM);
    
    printf("  -> PASS\n");
}

int main(void) {
    printf("Starting Unit Tests...\n");
    
    // Initialize our mocked queues
    rtos_system_init();

    // Initialize state manager and display
    initialize_state_manager();
    initialize_display_manager();
    
    // Run the tests
    test_alarm_button_overrides_view();
    
    // Add more tests here...
    
    printf("All tests passed successfully!\n");
    return 0;
}

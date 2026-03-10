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

void test_alarm_button_press(DisplayViewState_t expected_display_state) {
    StateMsg_t input_msg;
    DisplayMsg_t output_msg;
    bool received;
    
    //Create and process "button press" event message
    input_msg.event = EVENT_BUTTON_ALARM_PRESSED;
    state_manager_process_event(&input_msg);
    
    //Verify the output
    received = rtos_queue_receive(display_message_queue, &output_msg, 0);
    assert(received == true);
    assert(output_msg.event == DISPLAY_EVENT_SET_ACTIVE_VIEW);
    assert(output_msg.payload.view_ctrl.view == expected_display_state);

    printf("Verify display\n");
    display_manager_process_event(&output_msg);
    
    printf("  -> PASS\n");
}

void test_alarm_button_release(DisplayViewState_t expected_display_state) {
    StateMsg_t input_msg;
    DisplayMsg_t output_msg;
    bool received;

    //Create and process "button release" event message
    input_msg.event = EVENT_BUTTON_ALARM_RELEASED;
    state_manager_process_event(&input_msg);
    
    //Verify the output
    received = rtos_queue_receive(display_message_queue, &output_msg, 0);
    assert(received == true);
    assert(output_msg.event == DISPLAY_EVENT_SET_ACTIVE_VIEW);
    assert(output_msg.payload.view_ctrl.view == expected_display_state);

    printf("Verify display\n");
    display_manager_process_event(&output_msg);

    printf("  -> PASS\n");
}

void test_digit_button_press(DisplayViewState_t expected_display_state) {
    StateMsg_t input_msg;
    DisplayMsg_t output_msg;
    bool received;
    
    //Create and process "button press" event message
    input_msg.event = EVENT_BUTTON_DIGIT_PRESSED;
    state_manager_process_event(&input_msg);
    
    //Verify the output
    received = rtos_queue_receive(display_message_queue, &output_msg, 0);
    assert(received == true);
    assert(output_msg.event == DISPLAY_EVENT_SET_ACTIVE_VIEW);
    assert(output_msg.payload.view_ctrl.view == expected_display_state);

    printf("Verify display\n");
    display_manager_process_event(&output_msg);
    
    printf("  -> PASS\n");
}

void test_digit_button_release(DisplayViewState_t expected_display_state) {
    StateMsg_t input_msg;
    DisplayMsg_t output_msg;
    bool received;

    //Create and process "button release" event message
    input_msg.event = EVENT_BUTTON_DIGIT_RELEASED;
    state_manager_process_event(&input_msg);
    
    //Verify the output
    received = rtos_queue_receive(display_message_queue, &output_msg, 0);
    assert(received == true);
    assert(output_msg.event == DISPLAY_EVENT_SET_ACTIVE_VIEW);
    assert(output_msg.payload.view_ctrl.view == expected_display_state);

    printf("Verify display\n");
    display_manager_process_event(&output_msg);

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
    clear_display_queue();

    //alarm-press by itself
    test_alarm_button_press(VIEW_STATE_ALARM);
    test_alarm_button_release(VIEW_STATE_TIME);

    //digit-press by itself
    test_digit_button_press(VIEW_STATE_DIGIT);
    test_digit_button_release(VIEW_STATE_TIME);

    printf("\n\n");
    //alarm-press, digit-press, digit-release, alarm-release
    test_alarm_button_press(VIEW_STATE_ALARM);
    test_digit_button_press(VIEW_STATE_DIGIT);
    test_digit_button_release(VIEW_STATE_ALARM);
    test_alarm_button_release(VIEW_STATE_TIME);    
    
    // Add more tests here...
    
    printf("All tests passed successfully!\n");
    return 0;
}

#include <stdint.h>
#include <stdbool.h>

#include "os_abstraction.h"

typedef enum {
    VIEW_STATE_TIME,
    VIEW_STATE_ALARM,
    VIEW_STATE_DIGIT
} DisplayViewState_t;


typedef enum {
    DISPLAY_EVENT_UPDATE_TIME,
    DISPLAY_EVENT_UPDATE_ALARM_STATE,
    DISPLAY_EVENT_SET_ACTIVE_VIEW
} DisplayEvent_t;

// Message structure sent by the higher-level State Manager
typedef struct {
    DisplayEvent_t event;
    union {
        struct { uint8_t hours; uint8_t minutes; } time;
        struct { bool is_set; uint8_t hours; uint8_t minutes; } alarm;
        struct { DisplayViewState_t view; uint8_t digit_val; } view_ctrl;
    } payload;
} DisplayMsg_t;

extern RtosQueueHandle_t display_message_queue;

// Display manager task entry point
extern void display_manager_task();

#include <stdint.h>
#include <stdbool.h>

#include "os_abstraction.h"

extern void display_task(RtosQueueHandle_t msg_queue);

typedef enum {
    VIEW_STATE_TIME,
    VIEW_STATE_ALARM,
    VIEW_STATE_DIGIT
} DisplayViewState_t;

typedef enum {
    MSG_UPDATE_TIME,
    MSG_UPDATE_ALARM_STATE,
    MSG_SET_ACTIVE_VIEW
} DisplayMsgType_t;

// Message structure sent by the higher-level State Manager
typedef struct {
    DisplayMsgType_t type;
    union {
        struct { uint8_t hours; uint8_t minutes; } time;
        struct { bool is_set; uint8_t hours; uint8_t minutes; } alarm;
        struct { DisplayViewState_t view; uint8_t digit_val; } view_ctrl;
    } payload;
} DisplayMsg_t;

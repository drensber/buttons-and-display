#include <stdint.h>
#include <stdbool.h>

#include "os_abstraction.h"

extern void display_task(RtosQueueHandle_t msg_queue);

typedef enum {
    MSG_UPDATE_TIME,
    MSG_UPDATE_ALARM_STATE,
    MSG_CMD_SHOW_ALARM,
    MSG_CMD_SHOW_DIGIT
} DisplayMsgType_t;

// Message structure sent by the higher-level State Manager
typedef struct {
    DisplayMsgType_t type;
    union {
        struct { uint8_t hours; uint8_t minutes; } time;
        struct { bool is_set; uint8_t hours; uint8_t minutes; } alarm;
        uint8_t digit;
    } payload;
} DisplayMsg_t;



typedef enum {
    VIEW_STATE_DEFAULT_TIME,
    VIEW_STATE_TEMP_ALARM,
    VIEW_STATE_TEMP_DIGIT
} DisplayViewState_t;


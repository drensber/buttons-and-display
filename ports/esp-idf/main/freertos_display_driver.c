#include <stdio.h>
#include "hw_abstraction.h"

void disp_hw_clear(void) {
    printf("ESP32 Display: clear()\n"); 
}

void disp_hw_write_text(const char* text) {
    printf("ESP32 Display: write_text(\"%s\")\n", text); 
}

void disp_hw_set_upper_right_dot(bool enable) {
    printf("ESP32 Display: upper_right_dot(%s)\n", enable ? "true" : "false"); 
}

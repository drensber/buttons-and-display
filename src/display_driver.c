#include <stdio.h>
#include "hardware_abstraction_display.h"

void disp_hw_clear(void)
{
    printf("Calling disp_hw_clear()\n"); 
}

void disp_hw_write_text(const char* text)
{
    printf("Calling disp_hw_write_text(text=\"%s\")\n", text); 
}

void disp_hw_set_upper_right_dot(bool enable)
{
    printf("disp_hw_set_upper_right_dot(enable=\"%s\")\n",
	   enable ? "true" : "false"); 
}

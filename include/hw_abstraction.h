#include <stdbool.h>

// Clears the entire dot matrix display
extern void disp_hw_clear(void);

// Writes a formatted string to the dot matrix display (e.g., "12:30", "--:--")
extern void disp_hw_write_text(const char* text);

// Controls the specific hardware pixel in the upper right corner
extern void disp_hw_set_upper_right_dot(bool enable);

// Reads the physical state of the buttons (true = pressed, false = released)
extern bool hw_btn_read_alarm(void);
extern bool hw_btn_read_digit(void);

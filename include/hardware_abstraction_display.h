#include <stdbool.h>

// Clears the entire dot matrix display
extern void disp_hw_clear(void);

// Writes a formatted string to the dot matrix display (e.g., "12:30", "--:--")
extern void disp_hw_write_text(const char* text);

// Controls the specific hardware pixel in the upper right corner
extern void disp_hw_set_upper_right_dot(bool enable);

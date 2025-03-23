#include "control_neopixel_measure.h"
#include "loop.h"

extern global_vars_t global_var;

void control_neopixel_measure_page(char received_char) {
    switch (received_char) {
        case 'q':
        case 'Q':
            ansi_set_current_page(ANSI_PAGE_MAIN);
            dev_mode_change_mode(DEV_STATE_NONE);
            break;
        case 'm':
        case 'M':
            neopixel_send_color(global_var.visual_output, NEOPIXEL_RED);
            break;

        case 'n':
        case 'N':
            neopixel_send_color(global_var.visual_output, NEOPIXEL_BLUE);

            break;
        case 'b':
        case 'B': {
            neopixel_color_t p = {192, 255, 45};
            neopixel_set_color(global_var.visual_output, &p);
            break;
        }
        default:
            break;
    }
}

#include "control_levels.h"
#include "ansi_abstraction_layer.h"
#include "ansi_pages.h"
#include "global_vars.h"
#include "levels.h"
#include "loop.h"

extern global_vars_t global_var;

void control_levels_page(char received_char) {
    switch (received_char) {
        case 'q':
        case 'Q':
            ansi_clear_terminal();
            ansi_set_current_page(ANSI_PAGE_MAIN);
            dev_mode_change_mode(DEV_STATE_NONE);
            break;
        case '1':
            gpio_put(LED_PINS[0], (global_var.gpio_level_state[0] =
                                       !global_var.gpio_level_state[0]));
            break;
        case '2':
            gpio_put(LED_PINS[1], (global_var.gpio_level_state[1] =
                                       !global_var.gpio_level_state[1]));
            break;
        case '3':
            gpio_put(LED_PINS[2], (global_var.gpio_level_state[2] =
                                       !global_var.gpio_level_state[2]));
            break;
    }
}

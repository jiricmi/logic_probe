#include "control_shift_register.h"
#include <stdbool.h>
#include "advanced/shift_register.h"
#include "ansi_abstraction_layer.h"
#include "ansi_pages.h"
#include "loop.h"
#include "utils.h"

extern global_vars_t global_var;

void control_shift_register_page(char received_char) {
    switch (received_char) {
        case 'q':
        case 'Q':
            ansi_clear_terminal();
            ansi_set_current_page(ANSI_PAGE_MAIN);
            dev_mode_change_mode(DEV_STATE_NONE);
            break;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8': {
            uint8_t index = cdtoi(received_char);
            global_var.adv_shift_register->bits[index - 1] =
                !global_var.adv_shift_register->bits[index - 1];
            dev_mode_request_frontend_change();
            break;
        }
        case 's':
        case 'S':
            global_var.adv_shift_register->ready_to_send = true;
            break;
        default:
            break;
    }
}

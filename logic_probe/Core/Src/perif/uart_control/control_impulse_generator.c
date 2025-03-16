#include "control_impulse_generator.h"
#include <stdbool.h>
#include "global_vars.h"
#include "loop.h"
#include "utils.h"

extern global_vars_t global_var;

void control_impulse_generator_page(unsigned char received_char) {
    switch (received_char) {
        case 'q':
        case 'Q':
            ansi_set_current_page(ANSI_PAGE_MAIN);
            dev_mode_change_mode(DEV_STATE_NONE);
            generator_unapply_temp_period(global_var.signal_generator);
            break;
        case 't':
        case 'T':
            generator_change_mode(global_var.signal_generator);
            dev_mode_request_update();
            break;
        case 's':
        case 'S':
            generator_send_pulse(global_var.signal_generator);
            break;
        case 'e':
        case 'E':
            if (global_var.signal_generator->edit_period) {
                generator_apply_temp_period(global_var.signal_generator);
                dev_mode_request_update();
            } else {
                global_var.signal_generator->edit_period = true;
            }
            break;
        case '1':
        case '2':
        case '3':
           // if (!global_var.signal_generator->edit_period) {
           //     uint8_t n = (uint8_t)cdtoi((char)received_char);
           //     generator_toggle_signal(global_var.signal_generator, n);
           //     break;
           // }

        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '0': {
            sig_generator_t* sig_gen = global_var.signal_generator;
            _Bool edit_period = global_var.signal_generator->edit_period;
            _Bool count_digit = digits_count(sig_gen->period_us_temp) <= 7;

            if (edit_period && count_digit) {
                sig_gen->period_us_temp *= 10;
                sig_gen->period_us_temp += (uint32_t)cdtoi((char)received_char);
            }
            break;
        }
        case 'x':
        case 'X':
            if (global_var.signal_generator->edit_period) {
                global_var.signal_generator->period_us_temp /= 10;
            }
            break;
    }
}

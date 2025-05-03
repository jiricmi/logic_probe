#include "control_impulse_generator.h"
#include <stdbool.h>
#include "global_vars.h"
#include "loop.h"
#include "utils.h"

extern global_vars_t global_var;

void control_impulse_generator_page(unsigned char received_char) {
    sig_generator_t* generator = global_var.signal_generator;
    switch (received_char) {
        case 'q':
        case 'Q':
            if (generator->permanent_send) {
                break;
            }
            ansi_set_current_page(ANSI_PAGE_MAIN);
            dev_mode_change_mode(DEV_STATE_NONE);
            generator_unapply_temp_period(generator);
            break;
        case 't':
        case 'T':
            if (!generator->permanent_send) {
                generator_change_mode(generator);
                dev_mode_request_update();
            }
            break;
        case 's':
        case 'S':
            if (!generator->permanent_send) {
                generator->send = 1;
            }
            break;
        case 'e':
        case 'E':
            if (!generator->edit_repeat && !generator->permanent_send) {
                if (generator->edit_period) {
                    generator_apply_temp_period(generator);
                    dev_mode_request_update();
                } else {
                    generator->edit_period = true;
                }
            }
            dev_mode_request_frontend_change();

            break;
        case 'y':
        case 'Y':
            if (!generator->edit_period && !generator->permanent_send) {
                generator->edit_repeat = !generator->edit_repeat;
                if (!generator->edit_repeat && generator->repeat == 0) {
                    generator->repeat = 1;
                }
                dev_mode_request_frontend_change();
            }
            break;
        case 'D':
        case 'd':
            if (!generator->edit_repeat && !generator->edit_period) {
                generator->permanent_send = !generator->permanent_send;
                generator_send_permanent(generator);
            }
            break;

        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '0': {
            if (generator->edit_period) {
                _Bool count_digit =
                    digits_count(generator->period_us_temp) <= 6;

                if (count_digit) {
                    generator->period_us_temp *= 10;
                    generator->period_us_temp +=
                        (uint32_t)cdtoi((char)received_char);
                }
            } else if (generator->edit_repeat) {
                generator->repeat *= 10;
                generator->repeat += (uint8_t)cdtoi(received_char);
                if (generator->repeat > 64) {
                    generator->repeat = 64;
                }
            }
            dev_mode_request_frontend_change();
            break;
        }
        case 'x':
        case 'X':
            if (generator->edit_period) {
                generator->period_us_temp /= 10;
            } else if (generator->edit_repeat) {
                generator->repeat /= 10;
            }
            break;
    }
}

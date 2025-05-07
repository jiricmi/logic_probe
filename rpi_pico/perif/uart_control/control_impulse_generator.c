#include "control_impulse_generator.h"
#include <stdbool.h>
#include "ansi_abstraction_layer.h"
#include "global_vars.h"
#include "loop.h"
#include "signal_generation.h"
#include "utils.h"

extern global_vars_t global_var;

void control_impulse_generator_page(unsigned char received_char) {
    sig_gen_t* generator = &global_var.sig_gen_perif;
    switch (received_char) {
        case 'q':
        case 'Q':
            if (!generator->pwm_send) {
                ansi_clear_terminal();
                ansi_set_current_page(ANSI_PAGE_MAIN);
                dev_mode_change_mode(DEV_STATE_NONE);
            }
            break;
        case 't':
        case 'T':
            if (generator->mode == SIG_GEN_MODE_PULSE_UP) {
                generator->mode = SIG_GEN_MODE_PULSE_DOWN;
            } else {
                generator->mode = SIG_GEN_MODE_PULSE_UP;
            }
            dev_mode_request_update();
            break;
        case 'y':
        case 'Y':
            if (!generator->edit_period) {
                if (generator->edit_pwm) {
                    generator->edit_pwm = false;
                    if (generator->pwm_period < 2) {
                        generator->pwm_period = 2;
                    } else if (generator->pwm_period > 1000) {
                        generator->pwm_period = 1000;
                    }
                    dev_mode_request_update();
                } else {
                    generator->edit_pwm = true;
                }
            }
            dev_mode_request_frontend_change();
            break;
        case 'd':
        case 'D':
            if (!generator->edit_period && !generator->edit_pwm) {
                generator->pwm_send = !generator->pwm_send;
                if (generator->pwm_send) {
                    sig_gen_start_pwm(generator);
                } else {
                    sig_gen_stop_pwm(generator);
                    generator->pwm_send = false;
                    gpio_put(PWM_PIN, false);
                }
            }
            dev_mode_request_frontend_change();
            break;
        case 'u':
        case 'U':
            if (!generator->edit_period) {
                if (generator->edit_pwm) {
                    generator->pwm_duty += (generator->pwm_duty >= 100) ? 0 : 5;
                }
            }
            dev_mode_request_frontend_change();
            break;
        case 'i':
        case 'I':
            if (!generator->edit_period) {
                if (generator->edit_pwm) {
                    generator->pwm_duty -= (generator->pwm_duty == 0) ? 0 : 5;
                }
            }
            dev_mode_request_frontend_change();
            break;
        case 's':
        case 'S':
            generator->send = 1;
            break;
        case 'e':
        case 'E':
            if (!generator->edit_pwm) {
                if (generator->edit_period) {
                    generator->edit_period = false;
                    if (generator->period_us < 10) {
                        generator->period_us = 10;
                    } else if (generator->period_us > 1000) {
                        generator->period_us = 1000;
                    }

                    dev_mode_request_update();
                } else {
                    generator->edit_period = true;
                }
            }
            dev_mode_request_frontend_change();

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
                _Bool count_digit = digits_count(generator->period_us) <= 6;

                if (count_digit) {
                    generator->period_us *= 10;
                    generator->period_us +=
                        (uint32_t)cdtoi((char)received_char);
                }
            } else if (generator->edit_pwm) {
                _Bool count_digit = digits_count(generator->pwm_period) <= 3;

                if (count_digit) {
                    generator->pwm_period *= 10;
                    generator->pwm_period +=
                        (uint32_t)cdtoi((char)received_char);
                }
            }
            dev_mode_request_frontend_change();
            break;
        }
        case 'x':
        case 'X':
            if (generator->edit_period) {
                generator->period_us /= 10;
            } else if (generator->edit_pwm) {
                generator->pwm_period /= 10;
            }
            break;
            dev_mode_request_frontend_change();
    }
}

#include "control_voltage_measure.h"
#include "global_vars.h"
#include "loop.h"
#include "utils.h"

#include <stdbool.h>

extern global_vars_t global_var;
extern _Bool ansi_page_voltage_edit_resistance;

void control_voltage_page(const unsigned char received_char) {
    if (global_var.device_state == DEV_STATE_OHMMETER) {
        control_voltage_resistance(received_char);
    } else {
        control_voltage_measures(received_char);
    }
}

void control_voltage_base(const unsigned char received_char) {
    switch (received_char) {
        case 'q':
        case 'Q':
            ansi_set_current_page(ANSI_PAGE_MAIN);
            dev_mode_change_mode(DEV_STATE_NONE);
            break;
        case 'm':
        case 'M':
            if (global_var.device_state == DEV_STATE_VOLTMETER) {
                global_var.adc_vars->channel_state_unapplied[1] = true;
                adc_apply_channels(global_var.adc_vars);
                adc_setup_channel_struct(global_var.adc_vars);
                dev_mode_change_mode(DEV_STATE_OHMMETER);

            } else {
                dev_mode_change_mode(DEV_STATE_VOLTMETER);
            }
            break;
    }
}

void control_voltage_measures(const unsigned char received_char) {
    switch (received_char) {
        case '1':
        case '2':
        case '3': {
            int num = cdtoi(global_var.received_char);
            adc_flip_unapplied_channel(global_var.adc_vars, (size_t)num);
            break;
        }
        case 's':
        case 'S':
            adc_apply_channels(global_var.adc_vars);
            adc_setup_channel_struct(global_var.adc_vars);
            dev_mode_request_update();
            break;
        default:
            control_voltage_base(received_char);
    }
}

void control_voltage_resistance(const unsigned char received_char) {
    switch (received_char) {
        case 'e':
        case 'E':
            ansi_page_voltage_edit_resistance =
                !ansi_page_voltage_edit_resistance;
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
        case '0':
            if (ansi_page_voltage_edit_resistance &&
                digits_count(global_var.adc_vars->base_resistor) <= 7) {
                global_var.adc_vars->base_resistor *= 10;
                global_var.adc_vars->base_resistor +=
                    (uint32_t)cdtoi((char)received_char);
            }
            break;
        case 'x':
        case 'X':
            if (ansi_page_voltage_edit_resistance) {
                global_var.adc_vars->base_resistor /= 10;
            }
            break;
        default:
            control_voltage_base(received_char);
    }
}

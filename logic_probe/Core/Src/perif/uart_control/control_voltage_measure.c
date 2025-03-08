#include "control_voltage_measure.h"
#include "ansi_abstraction_layer.h"
#include "ansi_page_voltage_measure.h"
#include "global_vars.h"
#include "utils.h"

#include <stdbool.h>

extern global_vars_t global_var;
extern _Bool ansi_page_voltage_edit_resistance;

void control_voltage_page(const unsigned char received_char) {
    if (global_var.adc_vars->resistance_mode) {
        control_voltage_resistance(received_char);
    } else {
        control_voltage_measures(received_char);
    }
}

void control_voltage_base(const unsigned char received_char) {
    switch (received_char) {
        case 'q':
        case 'Q':
            adc_remove_unapplied_channels(global_var.adc_vars);
            ansi_clear_terminal();
            ansi_set_current_page(ANSI_PAGE_MAIN);
            break;
        case 'm':
        case 'M':
            global_var.adc_vars->resistance_mode =
                !global_var.adc_vars->resistance_mode;
            if (global_var.adc_vars->resistance_mode) {
                global_var.adc_vars->channel_state_unapplied[1] = true;
                adc_apply_channels(global_var.adc_vars);
                adc_setup_channel_struct(global_var.adc_vars);
            }
            ansi_clear_terminal();
            ansi_render_current_page();
            break;
    }
}

void control_voltage_measures(const unsigned char received_char) {
    switch (received_char) {
        case '1':
        case '2':
        case '3':
        case '4': {
            int num = cdtoi(global_var.received_char);
            if (num == -1) {
                // TODO: handle error
            }
            adc_flip_unapplied_channel(global_var.adc_vars, (size_t)num);
            ansi_render_voltage_measures(global_var.adc_vars);
            break;
        }
        case 's':
        case 'S':
            adc_apply_channels(global_var.adc_vars);
            adc_setup_channel_struct(global_var.adc_vars);
            ansi_clear_terminal();
            ansi_render_current_page();
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
            ansi_clear_terminal();
            ansi_render_current_page();
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

                ansi_clear_terminal();
                ansi_render_current_page();
            }
            break;
        case 'x':
        case 'X':
            if (ansi_page_voltage_edit_resistance) {
                global_var.adc_vars->base_resistor /= 10;
                ansi_clear_terminal();
                ansi_render_current_page();
            }
            break;
        default:
            control_voltage_base(received_char);
    }
}

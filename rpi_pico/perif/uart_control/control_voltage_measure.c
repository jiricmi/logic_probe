#include "control_voltage_measure.h"
#include "adc_control.h"
#include "global_structs.h"
#include "global_vars.h"
#include "loop.h"
#include "utils.h"

#include <stdbool.h>

extern global_vars_t global_var;

void control_voltage_page(const char received_char) {
    if (global_var.device_state == DEV_STATE_OHMMETER) {
        control_voltage_resistance(received_char);
    } else {
        control_voltage_base(received_char);
    }
}

void control_voltage_base(const char received_char) {
    adc_vars_t* perif = &global_var.adc_perif;
    switch (received_char) {
        case 'q':
        case 'Q':
            ansi_set_current_page(ANSI_PAGE_MAIN);
            dev_mode_change_mode(DEV_STATE_NONE);
            break;
        case 'm':
        case 'M':
            if (global_var.device_state == DEV_STATE_OHMMETER) {
                ansi_set_current_page(ANSI_PAGE_VOLTAGE_MEASURE);
                dev_mode_change_mode(DEV_STATE_VOLTMETER);
            } else {
                ansi_set_current_page(ANSI_PAGE_OHMMETER_MEASURE);
                dev_mode_change_mode(DEV_STATE_OHMMETER);
            }
            break;
    }
}

void control_voltage_resistance(const char received_char) {
    adc_vars_t* perif = &global_var.adc_perif;
    switch (received_char) {
        case 'e':
        case 'E':
            perif->edit_resistance = !perif->edit_resistance;
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
            if (perif->edit_resistance &&
                digits_count(perif->base_resistor) <= 7) {
                perif->base_resistor *= 10;
                perif->base_resistor += (uint32_t)cdtoi((char)received_char);
            }
            break;
        case 'x':
        case 'X':
            if (perif->edit_resistance) {
                perif->base_resistor /= 10;
            }
            break;
        default:
            control_voltage_base(received_char);
    }
}

#include "uart_control.h"
#include <stdbool.h>
#include "ansi_pages.h"
#include "control_frequency_reader.h"
#include "control_impulse_generator.h"
#include "control_levels.h"
#include "control_voltage_measure.h"
#include "global_vars.h"
#include "loop.h"

extern global_vars_t global_var;

void get_current_control(const char received_char) {
    if (received_char == 'r' || received_char == 'R') {
        dev_mode_request_frontend_change();
        return;
    }
    switch (global_var.current_page) {
        case ANSI_PAGE_MAIN:
            control_main_page(received_char);
            break;
        case ANSI_PAGE_VOLTAGE_MEASURE:
        case ANSI_PAGE_OHMMETER_MEASURE:
            control_voltage_page(received_char);
            break;
        case ANSI_PAGE_FREQUENCY_READER:
            control_frequency_reader_page(received_char,
                                          &global_var.sig_det_perif);
            break;
        case ANSI_PAGE_IMPULSE_GENERATOR:
            control_impulse_generator_page(received_char);
            break;
        case ANSI_PAGE_LEVELS:
            control_levels_page(received_char);
            break;

        default:
            control_main_page(received_char);
    }
}

void control_main_page(const char received_char) {
    switch (received_char) {
        case 'v':
        case 'V':
            ansi_set_current_page(ANSI_PAGE_VOLTAGE_MEASURE);
            dev_mode_change_mode(DEV_STATE_VOLTMETER);
            break;
        case 'f':
        case 'F':
            ansi_set_current_page(ANSI_PAGE_FREQUENCY_READER);
            dev_mode_change_mode(DEV_STATE_FREQUENCY_READ);
            break;
        case 'g':
        case 'G':
            ansi_set_current_page(ANSI_PAGE_IMPULSE_GENERATOR);
            dev_mode_change_mode(DEV_STATE_PULSE_GEN);
            break;
        case 'l':
        case 'L':
            ansi_set_current_page(ANSI_PAGE_LEVELS);
            dev_mode_change_mode(DEV_STATE_LEVEL);
            break;
    }
}

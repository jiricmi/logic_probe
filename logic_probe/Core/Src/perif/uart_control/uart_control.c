#include "uart_control.h"
#include <stdbool.h>
#include "ansi_pages.h"
#include "control_frequency_reader.h"
#include "control_impulse_generator.h"
#include "control_neopixel_measure.h"
#include "control_voltage_measure.h"
#include "global_vars.h"
#include "loop.h"

extern global_vars_t global_var;

void get_current_control(void) {
    char received_char = global_var.received_char;
    switch (global_var.current_page) {
        case ANSI_PAGE_MAIN:
            control_main_page();
            break;
        case ANSI_PAGE_VOLTAGE_MEASURE:
            control_voltage_page(received_char);
            break;
        case ANSI_PAGE_FREQUENCY_READER:
            control_frequency_reader_page(received_char,
                                          global_var.signal_detector);
            break;
        case ANSI_PAGE_IMPULSE_GENERATOR:
            control_impulse_generator_page(received_char);
            break;
        case ANSI_PAGE_NEOPIXEL_MEASURE:
            control_neopixel_measure_page(received_char);
            break;
        default:
            control_main_page();
    }
}

void control_main_page(void) {
    switch (global_var.received_char) {
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
        case 'n':
        case 'N':
            ansi_set_current_page(ANSI_PAGE_NEOPIXEL_MEASURE);
            dev_mode_change_mode(DEV_STATE_ADV_NEOPIXEL_READ);
            break;
    }
}

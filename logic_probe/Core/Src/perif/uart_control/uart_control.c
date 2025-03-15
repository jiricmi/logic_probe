#include "uart_control.h"
#include <stdbool.h>
#include "ansi_abstraction_layer.h"
#include "ansi_pages.h"
#include "control_frequency_reader.h"
#include "control_voltage_measure.h"
#include "global_vars.h"
#include "signal_generator.h"

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
            control_impulse_generator_page();
            break;

        default:
            control_main_page();
    }
}

void control_main_page(void) {
    switch (global_var.received_char) {
        case 'v':
        case 'V':
            ansi_clear_terminal();
            ansi_set_current_page(ANSI_PAGE_VOLTAGE_MEASURE);
            break;
        case 'f':
        case 'F':
            ansi_clear_terminal();
            ansi_set_current_page(ANSI_PAGE_FREQUENCY_READER);
            break;
        case 'g':
        case 'G':
            ansi_clear_terminal();
            ansi_set_current_page(ANSI_PAGE_IMPULSE_GENERATOR);
    }
}

void control_impulse_generator_page(void) {
    switch (global_var.received_char) {
        case 'q':
        case 'Q':
            ansi_clear_terminal();
            ansi_set_current_page(ANSI_PAGE_MAIN);
            break;
        case 'a':
        case 'A':
            sig_gen_change_period(global_var.signal_generator, -1);
            break;
        case 's':
        case 'S':
            sig_gen_change_period(global_var.signal_generator, 1);
            break;
        case 'd':
        case 'D':
            sig_gen_change_period(global_var.signal_generator, -10);
            break;
        case 'f':
        case 'F':
            sig_gen_change_period(global_var.signal_generator, 10);
            break;
        case 'g':
        case 'G':
            sig_gen_change_period(global_var.signal_generator, -100);
            break;
        case 'h':
        case 'H':
            sig_gen_change_period(global_var.signal_generator, 100);
            break;
        case 'w':
        case 'W':
            sig_gen_toggle_pulse(global_var.signal_generator, false);
            break;
        case 't':
        case 'T':
            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7);
            break;
        case 'y':
        case 'Y':
            sig_gen_toggle_pulse(global_var.signal_generator, true);
            break;
    }
}

#include "uart_control.h"
#include <stdbool.h>
#include "ansi_pages.h"
#include "control_frequency_reader.h"
#include "control_i2c.h"
#include "control_impulse_generator.h"
#include "control_neopixel_measure.h"
#include "control_shift_register.h"
#include "control_spi.h"
#include "control_uart_measure.h"
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
        case ANSI_PAGE_MAIN_ADVANCED:
            control_advanced_main_page();
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
        case ANSI_PAGE_SHIFT_REGISTER:
            control_shift_register_page(received_char);
            break;
        case ANSI_PAGE_UART:
            control_uart_page(received_char);
            break;
        case ANSI_PAGE_I2C:
            control_i2c_page(received_char);
            break;
        case ANSI_PAGE_SPI:
            control_spi_page(received_char);
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
        case 'a':
        case 'A':
            ansi_set_current_page(ANSI_PAGE_MAIN_ADVANCED);
            dev_mode_change_mode(DEV_STATE_NONE);
            break;
    }
}

void control_advanced_main_page(void) {
    switch (global_var.received_char) {
        case 'n':
        case 'N':
            ansi_set_current_page(ANSI_PAGE_NEOPIXEL_MEASURE);
            dev_mode_change_mode(DEV_STATE_ADV_NEOPIXEL_READ);
            break;
        case 's':
        case 'S':
            ansi_set_current_page(ANSI_PAGE_SHIFT_REGISTER);
            dev_mode_change_mode(DEV_STATE_ADV_SHIFT_REGISTER);
            break;
        case 'b':
        case 'B':
            ansi_set_current_page(ANSI_PAGE_MAIN);
            dev_mode_change_mode(DEV_STATE_NONE);
            break;
        case 'u':
        case 'U':
            ansi_set_current_page(ANSI_PAGE_UART);
            dev_mode_change_mode(DEV_STATE_ADV_UART_READ);
            break;
        case 'i':
        case 'I':
            ansi_set_current_page(ANSI_PAGE_I2C);
            dev_mode_change_mode(DEV_STATE_ADV_I2C_SCAN);
            break;
        case 'p':
        case 'P':
            ansi_set_current_page(ANSI_PAGE_SPI);
            dev_mode_change_mode(DEV_STATE_ADV_SPI_MASTER);
    }
}

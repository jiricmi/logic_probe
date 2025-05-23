#include "ansi_pages.h"

#include <stdbool.h>
#include <string.h>
#include "ansi_abstraction_layer.h"
#include "ansi_page_frequency_reader.h"
#include "ansi_page_i2c.h"
#include "ansi_page_impulse_generator.h"
#include "ansi_page_levels.h"
#include "ansi_page_main.h"
#include "ansi_page_shift_register.h"
#include "ansi_page_spi.h"
#include "ansi_page_uart.h"
#include "ansi_page_voltage_measure.h"
#include "ansi_pages_neopixel_measure.h"
#include "global_vars.h"

extern global_vars_t global_var;

void ansi_render_current_page(void) {
    switch (global_var.current_page) {
        case ANSI_PAGE_MAIN:
        case ANSI_PAGE_MAIN_ADVANCED:
            ansi_render_main_page();
            break;
        case ANSI_PAGE_VOLTAGE_MEASURE:
            ansi_render_voltage_page();
            break;
        case ANSI_PAGE_FREQUENCY_READER:
            ansi_render_frequency_reader_page();
            break;
        case ANSI_PAGE_IMPULSE_GENERATOR:
            ansi_render_impulse_generator_page();
            break;
        case ANSI_PAGE_LEVELS:
            ansi_render_levels_page();
            break;
        case ANSI_PAGE_NEOPIXEL_MEASURE:
            ansi_render_neopixel_measure_page();
            break;
        case ANSI_PAGE_SHIFT_REGISTER:
            ansi_render_shift_register_page();
            break;
        case ANSI_PAGE_UART:
            ansi_render_uart_measure_page();
            break;
        case ANSI_PAGE_I2C:
            ansi_render_i2c_measure_page();
            break;
        case ANSI_PAGE_SPI:
            ansi_render_spi_measure_page();
            break;
        default:
            ansi_render_main_page();
    }
}

void ansi_set_current_page(ansi_page_type_t page) {
    global_var.current_page = page;
}

void ansi_print_help_msg(const char* msg, const uint8_t row_offset) {
    ansi_set_cursor(TERMINAL_HEIGHT - 2 - row_offset,
                    TERMINAL_CENTER - (strlen(msg) / 2));
    ansi_send_text(msg, &ansi_default_conf);
}

void ansi_render_welcome_message(void) {
    ansi_render_border('r', "r", "");
    ansi_set_cursor(12, TERMINAL_CENTER - 9);
    ansi_send_text("Press 'R' to start", &ansi_bold_conf);
}

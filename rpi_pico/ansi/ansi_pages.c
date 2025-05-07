#include "ansi_pages.h"

#include <stdbool.h>
#include <string.h>
#include "ansi_abstraction_layer.h"
#include "global_vars.h"
#include "pages/ansi_page_frequency_reader.h"
#include "pages/ansi_page_impulse_generator.h"
#include "pages/ansi_page_levels.h"
#include "pages/ansi_page_main.h"
#include "pages/ansi_page_voltage_measure.h"

extern global_vars_t global_var;

void ansi_render_current_page(void) {
    switch (global_var.current_page) {
        case ANSI_PAGE_MAIN:
        case ANSI_PAGE_MAIN_ADVANCED:
            ansi_render_main_page();
            break;
        case ANSI_PAGE_VOLTAGE_MEASURE:
        case ANSI_PAGE_OHMMETER_MEASURE:
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
        default:
            ansi_render_main_page();
            break;
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

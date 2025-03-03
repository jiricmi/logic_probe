#include "ansi_pages.h"

#include "ansi_page_channel_settings.h"
#include "ansi_page_frequency_reader.h"
#include "ansi_page_impulse_generator.h"
#include "ansi_page_main.h"
#include "ansi_page_voltage_measure.h"

ansi_page_type_t current_page = ANSI_PAGE_MAIN;

void ansi_render_current_page(void) {
    switch (current_page) {
        case ANSI_PAGE_MAIN:
            ansi_render_main_page();
            break;
        case ANSI_PAGE_CHANNEL_SETTINGS:
            ansi_render_channel_settings();
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
        default:
            ansi_render_main_page();
    }
}

void ansi_set_current_page(ansi_page_type_t page) {
    current_page = page;
}

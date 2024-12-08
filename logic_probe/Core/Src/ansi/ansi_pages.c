#include "ansi_pages.h"

#include "adc_control.h"
#include "ansi_abstraction_layer.h"
#include "ansi_ascii_text.h"
#include "ansi_display.h"
#include "utils.h"

short current_page = MAIN_PAGE;
char* current_page_bg_color;
extern uint32_t v_ref;
extern uint32_t* v_measures;
extern adc_channels* adc1_ch;

void generate_menu(void) {
    const unsigned int center = TERMINAL_WIDTH / 2 - 10;
    unsigned int row = 13;
    ansi_print_button("r - reload page", "", "", row++, center);
    ansi_print_button("c - Channel settings", "", "", row++, center);
    ansi_print_button("v - Voltmeter", "", "", row++, center);
    ansi_print_button("f - Signal detect", "", "", row++, center);
    ansi_print_button("g - Signal generator", "", "", row++, center);
}

void generate_channel_menu(void) {
    const unsigned int center = TERMINAL_WIDTH / 2 - 9;
    unsigned int row = 13;

    for (int i = 0; i < NUM_CHANNELS; ++i) {
        char label[20];
        char num = itocd(i + 1);
        char pin = itocd((int)adc1_ch->pin[i]);
        snprintf(label, sizeof(label), "%c] CHANNEL %c (PA%c)", num, num, pin);
        ansi_print_button(label,
                          (adc1_ch->channel_unapplied[i]) ? GREEN_BG : RED_BG,
                          "", row, center);
        ++row;
    }

    if (!adc1_ch->applied) {
        ansi_set_cursor(row + 2, center - 12);
        ansi_send_text("Change was made! Press S to save settings.", RED_TEXT,
                       "", 0);
    } else {
        ansi_clear_line(row + 2, 1);
    }
    ansi_clear_format();
}

void ansi_main_page(void) {
    current_page = MAIN_PAGE;
    ansi_print_title(ASCII_MAIN_LOGO, RED_TEXT, "");
    ansi_set_cursor(9, 30);
    ansi_send_text("Made by Milan Jiříček", "", "", 1);
    ansi_print_border(BORDER_HORIZONTAL, BORDER_VERTICAL, "", "");
    generate_menu();
    ansi_home_cursor();
}

void ansi_channel_set_page(void) {
    current_page = CHANNEL_PAGE;
    ansi_print_border('#', "#", "", "");
    ansi_print_title(ASCII_CHANNEL, GREEN_TEXT, "");
    generate_channel_menu();
    ansi_home_cursor();
}

void ansi_voltage_page(void) {
    current_page = VOLTAGE_PAGE;
    ansi_print_border('@', "@", "", "");
    ansi_print_title(ASCII_VOLTAGE, MAGENTA_TEXT, "");
    ansi_print_voltage_measures(v_ref, v_measures, adc1_ch);
}

void ansi_frequency_reader_page(void) {
    current_page = IMPULSE_DETECTOR_PAGE;
    ansi_print_border('x', "x", "", "");
    ansi_print_title(ASCII_SIGNAL_DETECT, YELLOW_TEXT, "");
}

void ansi_impulse_generator_page(void) {
    current_page = IMPULSE_GENERATOR_PAGE;
    ansi_print_border('%', "%", "", "");
    ansi_print_title(ASCII_SIGNAL_GENERATOR, CYAN_TEXT, "");
}

void render_current_page(void) {
    switch (current_page) {
        case MAIN_PAGE:
            ansi_main_page();
            break;
        case CHANNEL_PAGE:
            ansi_channel_set_page();
            break;
        case VOLTAGE_PAGE:
            ansi_voltage_page();
            break;
        case IMPULSE_DETECTOR_PAGE:
            ansi_frequency_reader_page();
            break;
        case IMPULSE_GENERATOR_PAGE:
            ansi_impulse_generator_page();
            break;
        default:
            ansi_main_page();
    }
}

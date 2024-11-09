#include "ansi_pages.h"

#include "adc_control.h"
#include "ansi_abstraction_layer.h"
#include "ansi_ascii_text.h"
#include "ansi_display.h"
#include "my_bool.h"

short current_page = MAIN_PAGE;
char* current_page_bg_color;
extern uint32_t V_ref;
extern uint32_t channel_1_probe;
extern adc_channels* adc1_ch;

void generate_menu(void) {
    const unsigned int center = TERMINAL_WIDTH / 2 - 10;

    ansi_print_button("r - reload page", "", "", 13, center);
    ansi_print_button("c - Channel settings", "", "", 14, center);
    ansi_print_button("v - Voltmeter", "", "", 15, center);
}

void generate_channel_menu(void) {
    const unsigned int center = TERMINAL_WIDTH / 2 - 9;
    ansi_print_button("1] CHANNEL 1 (PA0)",
                      (adc1_ch->channel_1 == true) ? GREEN_BG : RED_BG, "", 13,
                      center);
    ansi_print_button("2] CHANNEL 2 (PA1)",
                      (adc1_ch->channel_2 == true) ? GREEN_BG : RED_BG, "", 14,
                      center);
    ansi_print_button("3] CHANNEL 3 (PA4)",
                      (adc1_ch->channel_3 == true) ? GREEN_BG : RED_BG, "", 15,
                      center);
    ansi_print_button("4] CHANNEL 4 (PA5)",
                      (adc1_ch->channel_4 == true) ? GREEN_BG : RED_BG, "", 16,
                      center);

    if (!adc1_ch->applied) {
        ansi_set_cursor(18, center - 12);
        ansi_send_text("Change was made! Press S to save settings.", RED_TEXT,
                       "", 0);
    } else {
        ansi_clear_line(18, 1);
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
    ansi_print_voltage_measures(V_ref, channel_1_probe, 0, 0, 0);
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
        default:
            ansi_main_page();
    }
}

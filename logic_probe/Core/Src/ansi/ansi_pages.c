#include "ansi_pages.h"

#include "ansi_abstraction_layer.h"
#include "ansi_ascii_text.h"
#include "ansi_display.h"

const char* voltage_ascii[] = {
    " __      __   _ _                   \n",
    " \\ \\    / /  | | |                  \n",
    "  \\ \\  / /__ | | |_ __ _  __ _  ___ \n",
    "   \\ \\/ / _ \\| | __/ _` |/ _` |/ _ \\\n",
    "    \\  / (_) | | || (_| | (_| |  __/\n",
    "     \\/ \\___/|_|\\__\\__,_|\\__, |\\___|\n",
    "                          __/ |     \n",
    "                         |___/       \n"};  // todo: udelat struct

unsigned char current_page = 0;
char* current_page_bg_color;
extern uint32_t V_ref;
extern uint32_t channel_1_probe;

void generate_menu(void) {
    const unsigned int center = TERMINAL_WIDTH / 2 - 10;

    ansi_print_button("r - reload page", "", "", 13, center);
    ansi_print_button("v - Voltmeter", "", "", 14, center);
}

void ansi_main_page(void) {
    current_page = 0;
    ansi_clear_terminal();
    ansi_print_title(ASCII_MAIN_LOGO, RED_TEXT, "");
    ansi_set_cursor(9, 30);
    ansi_send_text("Made by Milan Jiříček", "", "", 1);
    ansi_print_border(BORDER_HORIZONTAL, BORDER_VERTICAL, "", "");
    generate_menu();

    ansi_home_cursor();
}

void ansi_voltage_page(void) {
    current_page = 1;
    ansi_clear_terminal();
    ansi_print_border('@', "@", "", "");
    ansi_print_title(ASCII_VOLTAGE, MAGENTA_TEXT, "");
    ansi_print_voltage_measures(V_ref, channel_1_probe, 0, 0, 0);
}

void render_current_page(void) {
    switch (current_page) {
        case 0:
            ansi_main_page();
            break;
        case 1:
            ansi_voltage_page();
            break;
        default:
            ansi_main_page();
    }
}

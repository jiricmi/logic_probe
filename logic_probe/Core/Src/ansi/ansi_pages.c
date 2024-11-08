#include "ansi_pages.h"

#include "ansi_abstraction_layer.h"
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
    ansi_print_button("l - logic probe", "", "", 14, center);
}

void ansi_main_page(void) {
    current_page = 0;
    ansi_clear_terminal();
    ansi_print_logo(RED_TEXT, "");
    ansi_print_border(BORDER_HORIZONTAL, BORDER_VERTICAL, "", "");
    generate_menu();

    ansi_home_cursor();
}

void ansi_logic_probe_page(void) {
    current_page = 1;
    ansi_clear_terminal();
    ansi_print_border('@', "@", "", "");
    ansi_print_title(voltage_ascii, 8, 37, "", "");
    ansi_print_voltage_measures(V_ref, channel_1_probe, 0, 0, 0);
    ansi_home_cursor();
}

void render_current_page(void) {
    switch (current_page) {
        case 0:
            ansi_main_page();
            break;
        case 1:
            ansi_logic_probe_page();
            break;
        default:
            ansi_main_page();
    }
}

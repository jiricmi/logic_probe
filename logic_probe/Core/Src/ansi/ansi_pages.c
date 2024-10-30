#include "ansi_pages.h"

#include "ansi_abstraction_layer.h"
#include "ansi_display.h"

unsigned char current_page = 0;
char* current_page_bg_color;

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
    ansi_set_cursor(1, 1);
}

void ansi_logic_probe_page(void) {
    current_page = 1;
    ansi_clear_terminal();
    current_page_bg_color = LIGHT_BLUE_BG;
    ansi_print_border('@', "@", "", "");
    ansi_set_cursor(1, 1);
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

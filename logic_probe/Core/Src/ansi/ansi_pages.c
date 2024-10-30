#include "ansi_pages.h"

#include "ansi_abstraction_layer.h"
#include "ansi_display.h"

unsigned char current_page = 0;
char* current_page_bg_color;

void ansi_main_page(void) {
    ansi_clear_terminal();
    ansi_print_logo(RED_TEXT, "");
    ansi_print_border("", "");
}

void ansi_logic_probe(void) {
    current_page_bg_color = LIGHT_BLUE_BG;
    ansi_set_background(current_page_bg_color);
    ansi_print_logo("", "");
}

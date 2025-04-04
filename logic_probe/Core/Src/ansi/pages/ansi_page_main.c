#include "ansi_page_main.h"

#include "ansi_abstraction_layer.h"
#include "ansi_ascii_text.h"
#include "global_vars.h"

extern global_vars_t global_var;

void ansi_page_main_render_menu(void) {
    const unsigned int center = TERMINAL_WIDTH / 2 - 10;
    unsigned int row = 13;
    if (global_var.current_page == ANSI_PAGE_MAIN_ADVANCED) {
        ansi_render_button("r - reload page", row++, center,
                           &ansi_default_conf);
        ansi_render_button("b - Basic mode", row++, center, &ansi_default_conf);
        ansi_render_button("s - Shift register", row++, center,
                           &ansi_default_conf);
        ansi_render_button("n - Neopixel perif", row++, center,
                           &ansi_default_conf);
    } else {
        ansi_render_button("r - reload page", row++, center,
                           &ansi_default_conf);
        ansi_render_button("v - Voltmeter", row++, center, &ansi_default_conf);
        ansi_render_button("f - Signal detect", row++, center,
                           &ansi_default_conf);
        ansi_render_button("g - Signal generator", row++, center,
                           &ansi_default_conf);
        ansi_render_button("a - Advanced mode", row++, center,
                           &ansi_default_conf);
    }
}

void ansi_render_main_page(void) {
    ansi_render_title(ASCII_LOGO_MAIN, RED_TEXT);
    ansi_set_cursor(9, 30);
    ansi_send_text("Made by Milan Jiříček", &ansi_bold_conf);
    ansi_set_cursor(11, TERMINAL_WIDTH / 2 - 4);
    if (global_var.current_page == ANSI_PAGE_MAIN_ADVANCED) {
        ansi_send_text("ADVANCED MODE", &ansi_bold_conf);
    } else {
        ansi_send_text("BASIC MODE", &ansi_bold_conf);
    }
    ansi_set_cursor(20, TERMINAL_WIDTH / 2 - 12);
    ansi_send_text("Department of Measurement", &ansi_default_conf);
    ansi_set_cursor(21, TERMINAL_WIDTH / 2 - 4);
    ansi_send_text("CTU FEE", &ansi_default_conf);
    ansi_render_border(BORDER_HORIZONTAL, BORDER_VERTICAL, "");
    ansi_page_main_render_menu();
    ansi_home_cursor();
}

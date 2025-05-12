#include "ansi_page_levels.h"
#include <hardware/gpio.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "ansi_abstraction_layer.h"
#include "ansi_pages.h"
#include "levels.h"

void ansi_render_levels_page(void) {
    ansi_render_border('g', "g", "");
    const char* title = "GPIO LEVELS";
    ansi_set_cursor(4, TERMINAL_CENTER - (strlen(title) / 2));
    ansi_send_text(title, &ansi_bold_conf);

    char buff[50];
    uint8_t row = 8;
    for (uint8_t i = 0; i < sizeof(LED_PINS) / sizeof(LED_PINS[0]); ++i) {
        ansi_set_cursor(row++, TERMINAL_CENTER - 8);
        _Bool on = gpio_get(LED_PINS[i]);
        snprintf(buff, 50, "PIN GPIO%d: %s", LED_PINS[i], (on) ? "ON " : "OFF");
        if (on) {
            ansi_send_text(buff, &ansi_green_bold_conf);
        } else {
            ansi_send_text(buff, &ansi_red_bold_conf);
        }
    }
    ansi_print_help_msg("1-4: toggle output", 0);
}

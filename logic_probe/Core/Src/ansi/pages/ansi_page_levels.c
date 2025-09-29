#include "ansi_page_levels.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "ansi_abstraction_layer.h"
#include "ansi_pages.h"
#include "main.h"

void ansi_render_levels_page(void) {
    ansi_render_border('g', "g", "");
    const char* title = "GPIO LEVELS";
    ansi_set_cursor(4, TERMINAL_CENTER - (strlen(title) / 2));
    ansi_send_text(title, &ansi_bold_conf);

    uint16_t pins[] = {GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3};
    char pins_num[] = {'1', '2', '3'};
    uint8_t pins_real[] = {1, 9, 10};
    char buff[50];
    uint8_t row = 8;
    for (uint8_t i = 0; i < 1; ++i) {
        ansi_set_cursor(row++, TERMINAL_CENTER - 8);
        _Bool on = HAL_GPIO_ReadPin(GPIOA, pins[i]);
        snprintf(buff, 50, "PIN PA%c/A%d: %s", pins_num[i], pins_real[i],
                 (on) ? "ON " : "OFF");
        if (on) {
            ansi_send_text(buff, &ansi_green_bold_conf);
        } else {
            ansi_send_text(buff, &ansi_red_bold_conf);
        }
    }
    ansi_print_help_msg("1-3: toggle output", 0);
}

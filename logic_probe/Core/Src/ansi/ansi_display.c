#include "ansi_display.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ansi_abstraction_layer.h"

void ansi_set_background(const char* color) {
    for (unsigned int i = 1; i <= TERMINAL_HEIGHT; ++i) {
        ansi_set_cursor(i, 1);
        send_uart_string(color);
        for (unsigned int j = 1; j <= TERMINAL_WIDTH; ++j) {
            send_uart_string(" ");
        }
    }

    ansi_clear_format();
}

void ansi_print_logo(const char* color, const char* bg_color) {
    // clang-format off
    const char *jlp_art[] = {
        "    ___  ___       ________   \n",
        "   |\\  \\|\\  \\     |\\   __  \\  \n",
        "   \\ \\  \\ \\  \\    \\ \\  \\|\\  \\ \n",
        " __ \\ \\  \\ \\  \\    \\ \\   ____\\\n",
        "|\\  \\\\_\\  \\ \\  \\____\\ \\  \\___|\n",
        "\\ \\________\\ \\_______\\ \\__\\   \n",
        " \\|________|\\|_______|\\|__|   \n",
        "                               \n",
    };
    // clang-format on
    unsigned int row = 2;
    unsigned int art_rows = sizeof(jlp_art) / sizeof(jlp_art[0]);
    unsigned int width_center = TERMINAL_WIDTH / 2 - strlen(jlp_art[0]) / 2;

    for (unsigned i = 0; i < art_rows; ++i) {
        ansi_set_cursor(row + i, width_center);
        ansi_send_text(jlp_art[i], color, bg_color, 0);
    }

    char* made_by = "Made by Milan Jiříček";
    ansi_set_cursor(row + art_rows - 1,
                    TERMINAL_WIDTH / 2 - strlen(made_by) / 2);
    ansi_send_text(made_by, WHITE_TEXT, "", 0);

    ansi_clear_format();
}

void ansi_print_border(const char horizontal,
                       const char* vertical,
                       const char* color,
                       const char* bg_color) {
    char buffer[TERMINAL_WIDTH];

    memset(buffer, horizontal, sizeof(buffer));
    buffer[TERMINAL_WIDTH - 1] = '\0';

    ansi_set_cursor(0, 0);
    ansi_send_text(buffer, color, bg_color, 0);
    ansi_set_cursor(TERMINAL_HEIGHT - 1, 0);
    ansi_send_text(buffer, color, bg_color, 0);

    for (unsigned int i = 2; i < TERMINAL_HEIGHT - 1; ++i) {
        ansi_set_cursor(i, 0);
        ansi_send_text(vertical, color, bg_color, 0);
        ansi_set_cursor(i, TERMINAL_WIDTH - 1);
        ansi_send_text(vertical, color, bg_color, 0);
    }

    ansi_clear_format();
}

void ansi_print_button(const char* text,
                       const char* color_text,
                       const char* bg_color,
                       const unsigned int row,
                       const unsigned int col) {
    ansi_set_cursor(row, col);
    ansi_send_text("•", color_text, bg_color, 1);

    ansi_set_cursor(row, col + 2);
    ansi_send_text(text, color_text, bg_color, 1);
    ansi_clear_format();
}

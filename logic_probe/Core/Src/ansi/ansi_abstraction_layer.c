#include "ansi_abstraction_layer.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "ansi_ascii_text.h"
#include "main.h"

#define TEXT_SEND_BUFF_SIZE 500
#define CURSOR_BUFF_SIZE 40

extern UART_HandleTypeDef huart2;

void ansi_send_string(const char* str) {
    HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);
}

void ansi_send_text(const char* str, const ansi_text_config_t* text_conf) {
    char buffer[TEXT_SEND_BUFF_SIZE];
    size_t offset = 0;

    if (strlen(text_conf->bg_color) != 0) {
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "%s",
                           text_conf->bg_color);
    }

    if (strlen(text_conf->color) != 0) {
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "%s",
                           text_conf->color);
    }

    if (text_conf->bold) {
        offset +=
            snprintf(buffer + offset, sizeof(buffer) - offset, "%s", BOLD_TEXT);
    }

    offset += snprintf(buffer + offset, sizeof(buffer) - offset, "%s", str);

    if (offset >= sizeof(buffer)) {
        PrintError("Buffer overflow in text");
        return;
    }
    ansi_send_string(buffer);
    ansi_clear_format();
}

void ansi_home_cursor(void) {
    ansi_set_cursor(1, 1);
}

void ansi_clear_format(void) {
    ansi_send_string(ANSI_RESET_FORMAT);
}

void ansi_clear_terminal(void) {
    ansi_send_string(ANSI_CLEAR_SCREEN);
}

void ansi_clear_line(uint8_t row, uint8_t offset) {
    char buffer[TERMINAL_WIDTH];
    size_t last_el_index = TERMINAL_WIDTH - 2 * offset - 1;
    memset(buffer, ' ', last_el_index);
    buffer[last_el_index] = '\0';
    ansi_set_cursor(row, offset + 1);
    ansi_send_string(buffer);
}

void PrintError(const char* err_str) {
    ansi_set_cursor(30, 24);
    ansi_send_string(err_str);
    ansi_clear_format();
}

void ansi_set_cursor(const uint8_t row, const uint8_t col) {
    if (row > TERMINAL_HEIGHT || col > TERMINAL_WIDTH) {
        Error_Handler();
        return;
    }

    char result[CURSOR_BUFF_SIZE];

    size_t ret = snprintf(result, CURSOR_BUFF_SIZE, "\033[%u;%uH", row, col);

    if (ret >= sizeof(result)) {
        Error_Handler();
    }

    ansi_send_string(result);
}

void ansi_render_title(const unsigned short int ascii_type, char* color) {
    ascii_text* ascii = get_ascii_struct(ascii_type);

    size_t start_display_row = 2;
    size_t width_center = TERMINAL_WIDTH / 2 - ((ascii->col - 1) / 2);
    ansi_text_config_t conf = {color, "", 0};

    for (size_t i = 0; i < ascii->row; ++i) {
        ansi_set_cursor(start_display_row + i, width_center);
        ansi_send_text(ascii->ascii[i], &conf);
    }
    free_ascii_text(ascii);
    ansi_clear_format();
}

void ansi_render_border(const char horizontal,
                        const char* vertical,
                        char* color) {
    ansi_text_config_t border_conf = {color, "", 0};

    char buffer[TERMINAL_WIDTH];

    memset(buffer, horizontal, sizeof(buffer));
    buffer[TERMINAL_WIDTH - 1] = '\0';

    ansi_set_cursor(0, 0);
    ansi_send_text(buffer, &border_conf);
    ansi_set_cursor(TERMINAL_HEIGHT - 1, 0);
    ansi_send_text(buffer, &border_conf);

    for (uint8_t i = 2; i < TERMINAL_HEIGHT - 1; ++i) {
        ansi_set_cursor(i, 0);
        ansi_send_text(vertical, &border_conf);
        ansi_set_cursor(i, TERMINAL_WIDTH - 1);
        ansi_send_text(vertical, &border_conf);
    }

    ansi_clear_format();
}

void ansi_render_button(const char* text,
                        const uint8_t row,
                        const uint8_t col,
                        const ansi_text_config_t* conf) {
    ansi_set_cursor(row, col);
    ansi_send_text("•", conf);

    ansi_set_cursor(row, col + 2);
    ansi_send_text(text, conf);
    ansi_clear_format();
}

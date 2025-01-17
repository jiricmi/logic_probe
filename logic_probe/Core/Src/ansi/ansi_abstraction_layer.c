#include "ansi_abstraction_layer.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "my_bool.h"

#include "utils.h"

extern UART_HandleTypeDef huart2;

/**
 * @brief Sends string via UART to PC
 *
 * @param str String to send
 */
void ansi_send_string(const char* str) {
    HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);
}

/**
 * @brief Sends text with specific effects after
 *
 * @param str String to send
 * @param color macro from ansi_abstraction_layer.h representing color
 * @param bg_color macro from ansi_abstraction_layer.h representing bg color
 * @param bold make text bold
 */
void ansi_send_text(const char* str,
                    const char* color,
                    const char* bg_color,
                    const _Bool bold) {
    if (strlen(bg_color) != 0) {
        ansi_send_string(bg_color);
    }

    if (strlen(color) != 0) {
        ansi_send_string(color);
    }

    if (bold == true) {
        ansi_send_string(BOLD_TEXT);
    }

    ansi_send_string(str);
    ansi_clear_format();
}

void ansi_home_cursor(void) {
    ansi_set_cursor(1, 1);
}

void ansi_clear_format(void) {
    ansi_send_string("\033[0m\033[1;1H");
}

void ansi_clear_terminal(void) {
    ansi_send_string("\033[2J\033[H");
}

void ansi_clear_line(unsigned int row, unsigned int offset) {
    ansi_set_cursor(row, offset + 1);
    char buffer[TERMINAL_WIDTH];
    for (unsigned int i = 0; i < TERMINAL_WIDTH - 2 * offset - 1; ++i) {
        buffer[i] = ' ';
    }
    buffer[TERMINAL_WIDTH - 2 * offset - 1] = '\0';
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
    }

    char result[40];

    size_t ret = snprintf(result, 40, "\033[%u;%uH", row, col);

    if (ret >= sizeof(result) || ret < 0) {
        Error_Handler();
    }

    ansi_send_string(result);
}

#ifndef ANSI_ABSTRACTION_LAYER_H
#define ANSI_ABSTRACTION_LAYER_H

#include "main.h"

#define TERMINAL_WIDTH 80
#define TERMINAL_HEIGHT 24

#define BLACK_BG "\033[40m\0"
#define RED_BG "\033[41m\0"
#define GREEN_BG "\033[42m\0"
#define YELLOW_BG "\033[43m\0"
#define BLUE_BG "\033[44m\0"
#define MAGENTA_BG "\033[45m\0" 
#define CYAN_BG "\033[46m\0"
#define WHITE_BG "\033[47m\0"
#define DARK_GRAY_BG "\033[48;5;236m\0"
#define LIGHT_GRAY_BG "\033[48;5;245m\0"
#define DARK_BLUE_BG "\033[48;5;32m\0"
#define LIGHT_BLUE_BG "\033[48;5;68m\0"
#define DARK_GREEN_BG "\033[48;5;34m\0"
#define LIGHT_GREEN_BG "\033[48;5;84m\0"
#define DARK_YELLOW_BG "\033[48;5;184m\0"
#define LIGHT_YELLOW_BG "\033[48;5;226m\0"

#define BLACK_TEXT "\033[30m\0"
#define RED_TEXT "\033[31m\0"
#define GREEN_TEXT "\033[32m\0"
#define YELLOW_TEXT "\033[33m\0"
#define BLUE_TEXT "\033[34m\0"
#define MAGENTA_TEXT "\033[35m\0"
#define CYAN_TEXT "\033[36m\0"
#define WHITE_TEXT "\033[37m\0"

void send_uart_string(const char *str);
void ansi_send_text(const char *str, const char *color, const char *bg_color);
void ansi_clear_format();
void ansi_clear_terminal();
void PrintError(const char *err_str);
void ansi_set_cursor(const unsigned int row, const unsigned int col);
void int_to_string(char *buffer, const int number);
unsigned int join_strings(char *buffer, char *join, unsigned int start);
#endif // ANSI_ABSTRACTION_LAYER_H

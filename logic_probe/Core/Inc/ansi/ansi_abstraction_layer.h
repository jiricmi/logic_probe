#ifndef ANSI_ABSTRACTION_LAYER_H
#define ANSI_ABSTRACTION_LAYER_H

#include "main.h"

#define TERMINAL_WIDTH 80
#define TERMINAL_HEIGHT 24

void send_uart_string(const char *str);
void clear_ansi_format();
void PrintError(const char *err_str);
void ansi_set_cursor(const unsigned int row, const unsigned int col);
void int_to_string(char *buffer, const int number);
unsigned int join_strings(char *buffer, char *join, unsigned int start);
#endif // ANSI_ABSTRACTION_LAYER_H

#include "ansi_display.h"
#include "ansi_abstraction_layer.h"
#include <stdio.h>
#include <string.h>

void ansi_set_default_cursor(void) {
  send_uart_string("\033[2J");
  send_uart_string("\033[H");
}

void ansi_set_blue_background(void) {
  send_uart_string("\033[44m");
  ansi_set_default_cursor();
}

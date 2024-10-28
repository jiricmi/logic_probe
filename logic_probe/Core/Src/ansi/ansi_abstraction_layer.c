#include "ansi_abstraction_layer.h"
#include "utils.h"
#include <stdint.h>
#include <string.h>

extern UART_HandleTypeDef huart2;

void send_uart_string(const char *str) {
  HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen(str), HAL_MAX_DELAY);
}

void clear_ansi_format() { send_uart_string("\033[0m\033[1;1H"); }

void PrintError(const char *err_str) {
  ansi_set_cursor(30, 24);
  send_uart_string(err_str);
  clear_ansi_format();
}

void ansi_set_cursor(const unsigned int row, const unsigned int col) {
  if (row > TERMINAL_WIDTH || col > TERMINAL_HEIGHT) {
    PrintError("Cursor is out of range");
    return;
  }

  char result[20];
  unsigned int it = 0;
  result[it++] = '\033';
  result[it++] = '[';

  char row_buff[4];
  char col_buff[4];

  int_to_string(row_buff, row);
  int_to_string(col_buff, col);

  it = join_strings(result, row_buff, it);
  result[it++] = ';';
  it = join_strings(result, col_buff, it);
  result[it++] = 'H';
  result[it] = '\0';

  send_uart_string(result);
}

#include "ansi_abstraction_layer.h"
#include <string.h>

extern UART_HandleTypeDef huart2;

void send_uart_string(const char* str) {
  HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);
}

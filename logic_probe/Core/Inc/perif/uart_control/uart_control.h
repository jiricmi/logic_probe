#ifndef UART_CONTROL_H
#define UART_CONTROL_H
#include "stm32g031xx.h"

#define UART huart2
#define UART_INST USART2

void get_current_control(void);
void control_main_page(void);
void control_channel_set_page(void);
void control_advanced_main_page(void);
#endif

#ifndef UART_CONTROL_H
#define UART_CONTROL_H
#include "stm32g031xx.h"

#if defined(SOP8)
#define UART huart1
#define UART_INST USART1
#else
#define UART huart2
#define UART_INST USART2
#endif

void get_current_control(void);
void control_main_page(void);
void control_channel_set_page(void);
#endif

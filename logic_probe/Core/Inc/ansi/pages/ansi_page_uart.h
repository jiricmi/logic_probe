#ifndef ANSI_PAGE_UART_H
#define ANSI_PAGE_UART_H

#include "advanced/uart.h"

#define UART_BOX_WIDTH 20

void ansi_render_uart_measure_page(void);

void ansi_render_settings(uart_perif_t* uart);

void ansi_render_read_vals(uart_perif_t* uart);

void ansi_render_write_vals(uart_perif_t* uart);

#endif

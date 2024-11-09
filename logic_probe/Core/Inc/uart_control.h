#ifndef UART_CONTROL_H
#define UART_CONTROL_H
extern unsigned char received_char;
extern unsigned char current_page;

void get_current_control(void);
void control_main_page(void);
void control_voltage_page(void);

#endif

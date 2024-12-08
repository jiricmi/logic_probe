#ifndef UART_CONTROL_H
#define UART_CONTROL_H
extern unsigned char received_char;
extern short current_page;

void get_current_control(void);
void control_main_page(void);
void control_voltage_page(void);
void control_channel_set_page(void);
void control_frequency_reader_page(void);
void control_impulse_generator_page(void);
#endif

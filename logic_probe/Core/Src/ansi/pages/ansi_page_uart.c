#include "ansi_page_uart.h"
#include "advanced/uart.h"
#include "ansi_abstraction_layer.h"
#include "ansi_page_voltage_measure.h"
#include "global_vars.h"

extern global_vars_t global_var;

void ansi_render_uart_measure_page(void) {
    ansi_render_border(';', ";", "");
    if (global_var.device_state == DEV_STATE_ADV_UART_READ) {
        ansi_set_cursor(4, ADC_MEASURE_CENTER);
        ansi_send_text("UART READ (PIN PA3)", &ansi_bold_conf);
        ansi_render_settings(global_var.uart_perif);

    } else if (global_var.device_state == DEV_STATE_ADV_UART_WRITE) {
    }
}

void ansi_render_settings(uart_perif_t* uart) {
    ansi_set_cursor(6, ADC_MEASURE_CENTER - 10);
    char parity_char;
    if (uart->parity == UART_P_NONE) {
        parity_char = 'N';
    } else if (uart->parity == UART_P_EVEN) {
        parity_char = 'E';

    } else {
        parity_char = 'O';
    }
    char buffer[41];
    snprintf(buffer, 41, "WORDLEN: %d | PARITY: %c | STOPBITS: %d",
             uart->word_len + 7, parity_char, uart->stop_bits + 1);
    ansi_send_text(buffer, &ansi_bold_conf);
    ansi_set_cursor(7, ADC_MEASURE_CENTER);
    snprintf(buffer, 41, "BAUDRATE %lu", uart->baudrate);
    ansi_send_text(buffer, &ansi_bold_conf);
    if (uart->edit) {
        ansi_set_cursor(8, ADC_MEASURE_CENTER);
        ansi_text_config_t conf = {RED_TEXT, "", 1};
        ansi_send_text("CANNOT START UNTIL EDIT MODE!", &conf);
    }
}

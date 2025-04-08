#include "ansi_page_uart.h"
#include "advanced/uart.h"
#include "ansi_abstraction_layer.h"
#include "ansi_page_voltage_measure.h"
#include "global_vars.h"

extern global_vars_t global_var;

void ansi_render_uart_measure_page(void) {
    ansi_render_border(';', ";", "");
    ansi_render_settings(global_var.uart_perif);
    ansi_set_cursor(4, ADC_MEASURE_CENTER);
    if (global_var.device_state == DEV_STATE_ADV_UART_READ) {
        ansi_send_text("UART READ (PIN PA3)", &ansi_bold_conf);
        ansi_render_read_vals(global_var.uart_perif);
        ansi_uart_render_error(global_var.uart_perif);
    } else if (global_var.device_state == DEV_STATE_ADV_UART_WRITE) {
        ansi_send_text("UART WRITE (PIN PA2)", &ansi_bold_conf);
        ansi_render_write_vals(global_var.uart_perif);
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

void ansi_render_read_vals(uart_perif_t* uart) {
    uint16_t curr_buff_index =
        UART_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(uart->huart->hdmarx);

    char buff[11];
    uint8_t col = 0;
    uint8_t row = 12;
    ansi_set_cursor(row, 5);

    for (size_t i = 0; i < UART_BUFFER_SIZE; ++i) {
        if (curr_buff_index >= UART_BUFFER_SIZE) {
            curr_buff_index = 0;
        }
        if (uart->received_char[curr_buff_index] == 0) {
            ++curr_buff_index;
            continue;
        }

        ++col;
        snprintf(buff, 11, " %c(%3d)", uart->received_char[curr_buff_index],
                 uart->received_char[curr_buff_index]);
        ansi_send_text(buff, &ansi_default_conf);

        if (col >= 10) {
            col = 0;
            ++row;
            ansi_set_cursor(row, 5);
        }
        ++curr_buff_index;
    }
}

void ansi_render_write_vals(uart_perif_t* uart) {
    ansi_set_cursor(12, 10);
    ansi_text_config_t conf = {"", GREEN_BG, ""};
    char buff[4];
    for (uint8_t i = 0; i < UART_SEND_SIZE; ++i) {
        ansi_send_text(" | ", &ansi_default_conf);
        snprintf(buff, 4, "%3d", uart->received_char[i]);
        if (uart->edit_send && i == uart->edit_index) {
            ansi_send_text(buff, &conf);
        } else {
            ansi_send_text(buff, &ansi_default_conf);
        }
    }
    ansi_send_text(" |", &ansi_default_conf);
}

void ansi_uart_render_error(uart_perif_t* uart) {
    ansi_set_cursor(22, ADC_MEASURE_CENTER);
    ansi_text_config_t d = {RED_TEXT, "", 1};
    switch (uart->err_detected) {
        case UART_NONE_ERR:
            return;
        case UART_PARITY_ERR:
            ansi_send_text("PARITY ERROR!", &d);
            break;
        case UART_FRAME_ERR:
            ansi_send_text("FRAME ERROR!", &d);
            break;
        case UART_NOISE_ERR:
            ansi_send_text("NOISE ERROR!", &d);
            break;
        case UART_OVERRUN_ERR:
            ansi_send_text("OVERRUN ERROR!", &d);
            break;
        default:
            break;
    }
}

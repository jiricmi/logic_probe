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
        ansi_send_text("UART READ (PIN PA3/10)", &ansi_bold_conf);
        ansi_render_read_vals(global_var.uart_perif);
        help_read_uart();
    } else if (global_var.device_state == DEV_STATE_ADV_UART_WRITE) {
        ansi_send_text("UART WRITE (PIN PA2/9)", &ansi_bold_conf);
        ansi_render_write_vals(global_var.uart_perif);
        help_write_uart();
    }
}

void ansi_render_settings(uart_perif_t* uart) {
    ansi_set_cursor(6, ADC_MEASURE_CENTER - 12);
    char parity_char;
    if (uart->parity == UART_P_NONE) {
        parity_char = 'N';
    } else if (uart->parity == UART_P_EVEN) {
        parity_char = 'E';

    } else {
        parity_char = 'O';
    }
    char buffer[59];
    if (global_var.device_state == DEV_STATE_ADV_UART_WRITE) {
        snprintf(buffer, 59,
                 "WORDLEN: %d | PARITY: %c | STOPBITS: %d | SEND BYTES: %d",
                 uart->word_len + 7, parity_char, uart->stop_bits + 1,
                 uart->symbols_to_send);
    } else {
        snprintf(buffer, 59, "WORDLEN: %d | PARITY: %c | STOPBITS: %d",
                 uart->word_len + 7, parity_char, uart->stop_bits + 1);
    }
    ansi_send_text(buffer, &ansi_bold_conf);
    ansi_set_cursor(7, ADC_MEASURE_CENTER);
    snprintf(buffer, 41, "BAUDRATE %lu", uart->baudrate);
    ansi_send_text(buffer, &ansi_bold_conf);
    ansi_set_cursor(8, ADC_MEASURE_CENTER - 8);
    if (uart->edit) {
        ansi_send_text("CANNOT START UNTIL EDIT MODE!", &ansi_red_bold_conf);
    } else if (uart->edit_send) {
        ansi_send_text("CANNOT SEND UNTIL EDIT BYTES!", &ansi_red_bold_conf);
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

void help_read_uart(void) {
    if (global_var.uart_perif->edit) {
        ansi_print_help_msg("T: stop edit | 0-9: edit baudrate", 1);
        ansi_print_help_msg(
            "X: delete baudrate | Y: word len | U: parity | I: stop bit", 0);
    } else {
        ansi_print_help_msg("T: edit settings | M: change mode", 0);
    }
}

void help_write_uart(void) {
    if (global_var.uart_perif->edit) {
        ansi_print_help_msg(
            "T: stop edit | 0-9: edit baudrate | O: bytes count", 1);
        ansi_print_help_msg(
            "X: delete baudrate | Y: word len | U: parity | I: stop bit", 0);
    } else if (global_var.uart_perif->edit_send) {
        ansi_print_help_msg(
            "0-9: edit val | X: delete val | K: stop edit | L: move cursor", 0);

    } else {
        ansi_print_help_msg(
            "T: edit settings | K: edit vals | S: send | M: change mode", 0);
    }
}

#include "control_uart_measure.h"
#include "ansi_abstraction_layer.h"
#include "ansi_pages.h"
#include "global_vars.h"
#include "loop.h"
#include "utils.h"

extern global_vars_t global_var;

void control_uart_page(char received_char) {
    switch (received_char) {
        case 'q':
        case 'Q':
            ansi_clear_terminal();
            ansi_set_current_page(ANSI_PAGE_MAIN_ADVANCED);
            dev_mode_change_mode(DEV_STATE_NONE);
            break;
        case 'e':
        case 'E':
            global_var.uart_perif->edit = !global_var.uart_perif->edit;
            if (global_var.uart_perif->edit) {
                deinit_uart(global_var.uart_perif);
            } else {
                uart_start(global_var.uart_perif);
                uart_start_receive(global_var.uart_perif);
            }
            dev_mode_request_frontend_change();

            break;
        case 'w':
        case 'W':
            if (global_var.uart_perif->edit) {
                if (global_var.uart_perif->word_len < UART_WORD_LEN_LEN) {
                    ++global_var.uart_perif->word_len;
                } else {
                    global_var.uart_perif->word_len = UART_WORD_7B;
                }
            }
            dev_mode_request_frontend_change();

            break;
        case 'p':
        case 'P':
            if (global_var.uart_perif->edit) {
                if (global_var.uart_perif->parity < UART_PARITY_LEN) {
                    ++global_var.uart_perif->parity;
                } else {
                    global_var.uart_perif->parity = UART_P_NONE;
                }
            }
            dev_mode_request_frontend_change();

            break;
        case 'd':
        case 'D':
            if (global_var.uart_perif->edit) {
                if (global_var.uart_perif->stop_bits < UART_STOP_BIT_LEN) {
                    ++global_var.uart_perif->stop_bits;
                } else {
                    global_var.uart_perif->stop_bits = UART_STOP_1;
                }
            }
            dev_mode_request_frontend_change();

            break;
        case 'm':
        case 'M':

            break;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '0':
            if (global_var.uart_perif->edit &&
                digits_count(global_var.uart_perif->baudrate) <= 7) {
                global_var.uart_perif->baudrate *= 10;
                global_var.uart_perif->baudrate +=
                    (uint32_t)cdtoi((char)received_char);
            }
            dev_mode_request_frontend_change();

            break;
        case 'x':
        case 'X':
            if (global_var.uart_perif->edit) {
                global_var.uart_perif->baudrate /= 10;
            }
            dev_mode_request_frontend_change();

            break;

        default:
            break;
    }
}

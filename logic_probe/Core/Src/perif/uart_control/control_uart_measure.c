#include "control_uart_measure.h"
#include <string.h>
#include "ansi_abstraction_layer.h"
#include "ansi_pages.h"
#include "global_vars.h"
#include "loop.h"
#include "utils.h"

extern global_vars_t global_var;

void control_uart_page(char received_char) {
    if (global_var.uart_perif->edit_send) {
        control_uart_edit_page(received_char);
    } else {
        control_uart_nonedit_page(received_char);
    }
}

void control_uart_edit_page(char received_char) {
    uart_perif_t* perif = global_var.uart_perif;
    switch (received_char) {
        case ',':
            if (!perif->edit &&
                global_var.device_state == DEV_STATE_ADV_UART_WRITE) {
                perif->edit_send = !perif->edit_send;
                perif->edit_index = 0;
            }
            dev_mode_request_frontend_change();
            break;
        case '.':
            if (perif->edit_send &&
                global_var.device_state == DEV_STATE_ADV_UART_WRITE) {
                perif->edit_index++;
                if (perif->edit_index >= perif->symbols_to_send) {
                    perif->edit_index = 0;
                }
            }
            dev_mode_request_frontend_change();
            break;
        default:
            perif->received_char[perif->edit_index] = received_char;
            dev_mode_request_frontend_change();

            break;
    }
}

void control_uart_nonedit_page(char received_char) {
    uart_perif_t* perif = global_var.uart_perif;
    switch (received_char) {
        case 'q':
        case 'Q':
            if (!perif->edit_send && !perif->edit_index) {
                ansi_clear_terminal();
                ansi_set_current_page(ANSI_PAGE_MAIN_ADVANCED);
                dev_mode_change_mode(DEV_STATE_NONE);
            }
            break;
        case 't':
        case 'T':
            if (!perif->edit_send) {
                perif->edit = !global_var.uart_perif->edit;
                if (perif->edit) {
                    deinit_uart(perif);
                } else {
                    uart_start(perif);
                    uart_start_receive(perif);
                }
                dev_mode_request_frontend_change();
            }
            break;
        case 'y':
        case 'Y':
            if (perif->edit) {
                if (perif->word_len < UART_WORD_LEN_LEN) {
                    ++perif->word_len;
                } else {
                    perif->word_len = UART_WORD_7B;
                }
            }
            dev_mode_request_frontend_change();

            break;
        case 'u':
        case 'U':
            if (perif->edit) {
                if (perif->parity < UART_PARITY_LEN) {
                    ++perif->parity;
                } else {
                    perif->parity = UART_P_NONE;
                }
            }
            dev_mode_request_frontend_change();
            break;
        case 'g':
        case 'G':
            memset(perif->received_char, 0, sizeof(perif->received_char));
            dev_mode_update_perif();
            dev_mode_request_frontend_change();
            break;
        case 'i':
        case 'I':
            if (perif->edit) {
                if (perif->stop_bits < UART_STOP_BIT_LEN) {
                    ++perif->stop_bits;
                } else {
                    perif->stop_bits = UART_STOP_1;
                }
            }
            dev_mode_request_frontend_change();

            break;
        case 'm':
        case 'M':
            if (!perif->edit) {
                ansi_clear_terminal();
                if (global_var.device_state == DEV_STATE_ADV_UART_READ) {
                    dev_mode_change_mode(DEV_STATE_ADV_UART_WRITE);
                } else {
                    dev_mode_change_mode(DEV_STATE_ADV_UART_READ);
                }
            }
            break;
        case ',':
            if (!perif->edit &&
                global_var.device_state == DEV_STATE_ADV_UART_WRITE) {
                perif->edit_send = !perif->edit_send;
                perif->edit_index = 0;
            }
            dev_mode_request_frontend_change();
            break;
        case 's':
        case 'S':
            if (!perif->edit_send && !perif->edit &&
                global_var.device_state == DEV_STATE_ADV_UART_WRITE) {
                uart_send_bytes(perif);
            }
            dev_mode_request_frontend_change();
            break;
        case 'o':
        case 'O':
            if (global_var.device_state == DEV_STATE_ADV_UART_WRITE) {
                perif->symbols_to_send++;
                if (perif->symbols_to_send > UART_SEND_SIZE) {
                    perif->symbols_to_send = 1;
                }
            }
            dev_mode_request_frontend_change();
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
        case '0': {
            if (perif->edit && digits_count(perif->baudrate) <= 7) {
                perif->baudrate *= 10;
                perif->baudrate += (uint32_t)cdtoi((char)received_char);
            }
            dev_mode_request_frontend_change();

            break;
        }
        case 'x':
        case 'X':
            if (perif->edit) {
                perif->baudrate /= 10;
            }
            dev_mode_request_frontend_change();

            break;

        default:
            break;
    }
}

#include "control_spi.h"
#include "ansi_abstraction_layer.h"
#include "ansi_pages.h"
#include "global_vars.h"
#include "loop.h"
#include "utils.h"

extern global_vars_t global_var;

void control_spi_page(char received_char) {
    spi_perif_t* perif = global_var.spi_perif;
    switch (received_char) {
        case 'q':
        case 'Q':
            ansi_clear_terminal();
            ansi_set_current_page(ANSI_PAGE_MAIN_ADVANCED);
            dev_mode_change_mode(DEV_STATE_NONE);
            break;
        case 't':
        case 'T':
            if (!perif->edit_vals) {
                perif->edit_settings = !perif->edit_settings;
            }
            perif->error = SPI_ERROR_NONE;
            dev_mode_request_frontend_change();
            break;
        case 'u':
        case 'U':
            if (perif->edit_settings) {
                perif->CLK_polarity = !perif->CLK_polarity;
                dev_mode_request_frontend_change();
            }
            break;
        case 'y':
        case 'Y':
            if (perif->edit_settings) {
                perif->bytes_count++;
                if (perif->bytes_count > SPI_ARRAY_SIZE) {
                    perif->bytes_count = 1;
                }
                dev_mode_request_frontend_change();
            }
            break;
        case 'i':
        case 'I':
            if (perif->edit_settings) {
                perif->CLK_phase = !perif->CLK_phase;
                dev_mode_request_frontend_change();
            }
            break;
        case 'l':
        case 'L':
            if (!perif->edit_settings) {
                perif->edit_vals = !perif->edit_vals;
            }
            dev_mode_request_frontend_change();
            break;
        case 'k':
        case 'K':
            if (perif->edit_vals) {
                perif->master_index++;
                if (perif->master_index == I2C_ARRAY_SIZE) {
                    perif->master_index = 0;
                }
            }
            dev_mode_request_frontend_change();
            break;
        case 'o':
        case 'O':
            if (perif->edit_settings) {
                perif->msb = !perif->msb;
                dev_mode_request_frontend_change();
            }
            break;
        case 'p':
        case 'P':
            if (perif->edit_settings) {
                perif->read_bit = !perif->read_bit;
                dev_mode_request_frontend_change();
            }
            break;
        case 'm':
        case 'M': {
            perif->error = SPI_ERROR_NONE;
            if (!perif->edit_settings && !perif->edit_vals) {
                dev_state_t mode = global_var.device_state;
                if (mode == DEV_STATE_ADV_SPI_TEST_DISPLAY) {
                    mode = DEV_STATE_ADV_SPI_SLAVE;
                } else {
                    mode++;
                }
                dev_mode_change_mode(mode);
            }
            break;
        }
        case 's':
        case 'S':
            if (!perif->edit_vals && !perif->edit_settings) {
                perif->send_data = 1;
            }
            break;
        case 'a':
        case 'A':
        case 'b':
        case 'B':
        case 'c':
        case 'C':
        case 'd':
        case 'D':
        case 'e':
        case 'E':
        case 'f':
        case 'F': {
            if (perif->edit_vals && !perif->read_bit &&
                perif->data[perif->master_index] < 16) {
                perif->data[perif->master_index] *= 16;
                perif->data[perif->master_index] += char_to_hex(received_char);
            } else if (perif->edit_vals && perif->read_bit &&
                       perif->master_send_data[0] < 16) {
                perif->master_send_data[0] *= 16;
                perif->master_send_data[0] += char_to_hex(received_char);
            }
            dev_mode_request_frontend_change();
            break;
        }
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
            if (perif->edit_vals && !perif->read_bit &&
                perif->data[perif->master_index] < 16) {
                perif->data[perif->master_index] *= 16;
                perif->data[perif->master_index] += cdtoi(received_char);
            } else if (perif->edit_vals && perif->read_bit &&
                       perif->master_send_data[0] < 16) {
                perif->master_send_data[0] *= 16;
                perif->master_send_data[0] += cdtoi(received_char);
            }

            dev_mode_request_frontend_change();
            break;
        }
        case 'x':
        case 'X':
            if (perif->edit_vals && !perif->read_bit) {
                perif->data[perif->master_index] /= 16;
            } else if (perif->edit_vals && perif->read_bit) {
                perif->master_send_data[0] /= 16;
            }

            dev_mode_request_frontend_change();
            break;
    }
}

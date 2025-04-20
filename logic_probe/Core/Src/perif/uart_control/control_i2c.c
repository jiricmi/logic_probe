#include "control_i2c.h"
#include <string.h>
#include "ansi_abstraction_layer.h"
#include "ansi_pages.h"
#include "global_structs.h"
#include "global_vars.h"
#include "loop.h"
#include "utils.h"

extern global_vars_t global_var;

void control_i2c_page(char received_char) {
    i2c_perif_t* perif = global_var.i2c_perif;
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
                if (perif->edit_settings) {
                    i2c_deinit_perif(perif);
                } else {
                    i2c_init_perif(perif);
                    i2c_start_slave_listen(perif);
                }
                perif->send_status = I2C_NONE;
            }
            dev_mode_request_frontend_change();
            break;
        case 'y':
        case 'Y':
            if (perif->edit_settings) {
                perif->bytes_to_catch++;
                if (perif->bytes_to_catch > I2C_ARRAY_SIZE) {
                    perif->bytes_to_catch = 1;
                }
            }
            dev_mode_request_frontend_change();
            break;
        case 'm':
        case 'M': {
            dev_state_t mode = global_var.device_state;
            if (mode == DEV_STATE_ADV_I2C_TEST_DISPLAY) {
                dev_mode_change_mode(DEV_STATE_ADV_I2C_SCAN);
            } else {
                ++mode;
                dev_mode_change_mode(mode);
            }
            perif->send_status = I2C_NONE;
            dev_mode_request_frontend_change();
            break;
        }
        case 'u':
        case 'U':
            if (perif->edit_settings) {
                perif->read_bit = !perif->read_bit;
                memset(perif->slave_received_data, 0, I2C_ARRAY_SIZE);
            }
            dev_mode_request_frontend_change();
            break;
        case 'i':
        case 'I':
            if (!perif->edit_settings) {
                perif->edit_vals = !perif->edit_vals;
            }
            perif->send_status = I2C_NONE;
            dev_mode_request_frontend_change();
            break;
        case 'o':
        case 'O':
            if (perif->edit_vals) {
                perif->master_index++;
                if (perif->master_index == I2C_ARRAY_SIZE) {
                    perif->master_index = 0;
                }
            }
            break;
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
            if (perif->edit_settings && perif->slave_address < 16) {
                perif->slave_address *= 16;
                perif->slave_address += char_to_hex(received_char);
                if (perif->slave_address > 127) {
                    perif->slave_address = 127;
                }
            } else if (perif->edit_vals &&
                       perif->slave_received_data[perif->master_index] < 16) {
                perif->slave_received_data[perif->master_index] *= 16;
                perif->slave_received_data[perif->master_index] +=
                    char_to_hex(received_char);
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
            if (perif->edit_settings && perif->slave_address < 16) {
                perif->slave_address *= 16;
                perif->slave_address += cdtoi(received_char);
                if (perif->slave_address > 127) {
                    perif->slave_address = 127;
                }
            } else if (perif->edit_vals &&
                       perif->slave_received_data[perif->master_index] < 16) {
                perif->slave_received_data[perif->master_index] *= 16;
                perif->slave_received_data[perif->master_index] +=
                    cdtoi(received_char);
            }
            dev_mode_request_frontend_change();
            break;
        }
        case 'x':
        case 'X':
            if (perif->edit_settings) {
                perif->slave_address /= 16;
            } else if (perif->edit_vals) {
                perif->slave_received_data[perif->master_index] /= 16;
            }
            dev_mode_request_frontend_change();
            break;
    }
}

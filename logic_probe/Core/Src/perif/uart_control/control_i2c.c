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
            if (!perif->edit_vals && !perif->edit_settings) {
                ansi_clear_terminal();
                ansi_set_current_page(ANSI_PAGE_MAIN_ADVANCED);
                dev_mode_change_mode(DEV_STATE_NONE);
            }
            break;
        case 't':
        case 'T':
            if (!perif->edit_vals &&
                global_var.device_state != DEV_STATE_ADV_I2C_MONITOR) {
                perif->edit_settings = !perif->edit_settings;
                if (perif->edit_settings) {
                    i2c_deinit_perif(perif);
                } else {
                    i2c_init_perif(perif);
                    dev_mode_update_perif();
                }
                perif->send_status = I2C_NONE;
            }
            dev_mode_request_frontend_change();
            break;
        case 'g':
        case 'G':
            if (!perif->edit_vals && !perif->edit_settings) {
                memset(perif->slave_received_data, 0,
                       sizeof(perif->slave_received_data));
                memset(perif->master_read_send_data, 0,
                       sizeof(perif->master_read_send_data));
                memset(perif->monitor_data, 0, I2C_MONITOR_SIZE);
                dev_mode_update_perif();
                dev_mode_request_frontend_change();
            }
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
            if (!perif->edit_vals && !perif->edit_settings) {
                perif->send_status = I2C_NONE;
                dev_state_t mode = global_var.device_state;
                if (mode == DEV_STATE_ADV_I2C_TEST_DISPLAY) {
                    dev_mode_change_mode(DEV_STATE_ADV_I2C_SCAN);
                } else {
                    ++mode;
                    dev_mode_change_mode(mode);
                }
            }
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
        case 'k':
        case 'K':
            if (!perif->edit_settings &&
                global_var.device_state == DEV_STATE_ADV_I2C_MASTER) {
                perif->edit_vals = !perif->edit_vals;
            }
            perif->send_status = I2C_NONE;
            dev_mode_request_frontend_change();
            break;
        case 'l':
        case 'L':
            if (perif->edit_vals) {
                perif->master_index++;
                if (perif->master_index >= perif->bytes_to_catch) {
                    perif->master_index = 0;
                }
            }
            dev_mode_request_frontend_change();
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
            } else if (perif->edit_vals && !perif->read_bit &&
                       perif->slave_received_data[perif->master_index] < 16) {
                perif->slave_received_data[perif->master_index] *= 16;
                perif->slave_received_data[perif->master_index] +=
                    char_to_hex(received_char);
            } else if (perif->edit_vals && perif->read_bit &&
                       perif->master_read_send_data[0] < 16) {
                perif->master_read_send_data[0] *= 16;
                perif->master_read_send_data[0] += char_to_hex(received_char);
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
            } else if (perif->edit_vals && !perif->read_bit &&
                       perif->slave_received_data[perif->master_index] < 16) {
                perif->slave_received_data[perif->master_index] *= 16;
                perif->slave_received_data[perif->master_index] +=
                    cdtoi(received_char);
            } else if (perif->edit_vals && perif->read_bit &&
                       perif->master_read_send_data[0] < 16) {
                perif->master_read_send_data[0] *= 16;
                perif->master_read_send_data[0] += cdtoi(received_char);
            }

            dev_mode_request_frontend_change();
            break;
        }
        case 'x':
        case 'X':
            if (perif->edit_settings) {
                perif->slave_address /= 16;
            } else if (perif->edit_vals && !perif->read_bit) {
                perif->slave_received_data[perif->master_index] /= 16;
            } else if (perif->edit_vals && perif->read_bit) {
                perif->master_read_send_data[0] /= 16;
            }

            dev_mode_request_frontend_change();
            break;
    }
}

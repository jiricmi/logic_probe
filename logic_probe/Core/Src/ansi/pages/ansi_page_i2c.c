#include "ansi_page_i2c.h"
#include <string.h>
#include "advanced/i2c.h"
#include "ansi_abstraction_layer.h"
#include "ansi_pages.h"
#include "global_structs.h"
#include "global_vars.h"
#include "loop.h"
#include "stm32g0xx_hal_def.h"

#define I2C_BUFFER_SIZE 50
#define MAX_ADDRESSES 127
#define EDIT_MESSAGE "Editing I2C settings!"

extern global_vars_t global_var;

static void render_edit_status(uint8_t edit_flag) {
    if (edit_flag) {
        ansi_set_cursor(7, TERMINAL_CENTER - 10);
        ansi_send_text(EDIT_MESSAGE, &ansi_red_bold_conf);
    }
}

void ansi_render_i2c_measure_page(void) {
    ansi_render_border('&', "&", "");

    const char* header = "SCL - PB8 | SDA - PB9";
    ansi_set_cursor(5, TERMINAL_CENTER - (strlen(header) / 2));
    ansi_send_text(header, &ansi_bold_conf);

    const uint8_t state = global_var.device_state;
    i2c_perif_t* perif = global_var.i2c_perif;

    if (perif == NULL)
        return;

    const char* title = "";
    switch (state) {
        case DEV_STATE_ADV_I2C_SLAVE:
            title = "I2C SLAVE";
            ansi_i2c_render_settings(perif);
            ansi_i2c_render_read_vals(perif);
            help_slave_i2c();
            break;

        case DEV_STATE_ADV_I2C_MASTER:
            title = "I2C MASTER";
            ansi_i2c_master_render_settings(perif);
            ansi_i2c_master_vals(perif);
            help_master_i2c();
            break;

        case DEV_STATE_ADV_I2C_SCAN:
            title = "I2C SCAN ADDRESS";
            ansi_i2c_render_scan(perif);
            help_scan_i2c();
            break;

        case DEV_STATE_ADV_I2C_TEST_DISPLAY:
            title = "I2C TEST SSD1306";
            ansi_i2c_test_display_render_settings(perif);
            help_display_i2c();
            break;
    }

    if (*title != '\0') {
        ansi_set_cursor(4, TERMINAL_CENTER - (strlen(title) / 2));
        ansi_send_text(title, &ansi_bold_conf);
    }
}

void ansi_i2c_render_settings(i2c_perif_t* i2c_perif) {
    char buff[I2C_BUFFER_SIZE];
    snprintf(buff, sizeof(buff),
             "Slave address: 0x%02X | Bytes to receive: %2d",
             i2c_perif->slave_address, i2c_perif->bytes_to_catch);
    ansi_set_cursor(6, TERMINAL_CENTER - (strlen(buff) / 2));
    ansi_send_text(buff, &ansi_default_conf);
    render_edit_status(i2c_perif->edit_settings);
}

void ansi_i2c_master_render_settings(i2c_perif_t* i2c_perif) {
    char buff[I2C_BUFFER_SIZE];
    snprintf(buff, sizeof(buff), "Address: 0x%02X | %s | Bytes to %s: %2d",
             i2c_perif->slave_address, i2c_perif->read_bit ? "read" : "write",
             i2c_perif->read_bit ? "read" : "write", i2c_perif->bytes_to_catch);
    ansi_set_cursor(6, TERMINAL_CENTER - (strlen(buff) / 2));
    ansi_send_text(buff, &ansi_default_conf);
    render_edit_status(i2c_perif->edit_settings);
}

void ansi_i2c_test_display_render_settings(i2c_perif_t* i2c_perif) {
    char buff[I2C_BUFFER_SIZE];
    snprintf(buff, sizeof(buff), "Address: 0x%02X", i2c_perif->slave_address);
    ansi_set_cursor(6, TERMINAL_CENTER - (strlen(buff) / 2));
    ansi_send_text(buff, &ansi_default_conf);
    render_edit_status(i2c_perif->edit_settings);

    const char* msg1 = "Press S to start checking display...";
    ansi_set_cursor(8, TERMINAL_CENTER - (strlen(msg1) / 2));
    ansi_send_text(msg1, &ansi_bold_conf);

    const char* msg2 = "Display should show all pixels....";
    ansi_set_cursor(9, TERMINAL_CENTER - (strlen(msg2) / 2));
    ansi_send_text(msg2, &ansi_bold_conf);
}

void ansi_i2c_master_vals(i2c_perif_t* i2c_perif) {
    char buff[6];
    const uint8_t base_row = 8;
    const uint8_t base_col = TERMINAL_CENTER;

    if (i2c_perif->read_bit) {
        snprintf(buff, sizeof(buff), "0x%02X",
                 i2c_perif->master_read_send_data[0]);
        ansi_set_cursor(base_row, base_col - (strlen(buff) / 2));
        ansi_send_text(buff, i2c_perif->edit_vals ? &ansi_green_bold_conf
                                                  : &ansi_default_conf);

        ansi_set_cursor(10, base_col - 5 - (3 * i2c_perif->bytes_to_catch));
        for (uint8_t i = 0; i < i2c_perif->bytes_to_catch; ++i) {
            ansi_send_text(" | ", &ansi_default_conf);
            snprintf(buff, sizeof(buff), "0x%02X ",
                     i2c_perif->slave_received_data[i]);
            ansi_send_text(buff, &ansi_default_conf);
        }
        ansi_send_text(" | ", &ansi_default_conf);

    } else {
        ansi_set_cursor(10, base_col - 5 - (3 * i2c_perif->bytes_to_catch));
        for (uint8_t i = 0; i < i2c_perif->bytes_to_catch; ++i) {
            ansi_send_text(" | ", &ansi_default_conf);
            snprintf(buff, sizeof(buff), "0x%02X",
                     i2c_perif->slave_received_data[i]);
            ansi_send_text(
                buff, (i2c_perif->edit_vals && i == i2c_perif->master_index)
                          ? &ansi_green_bold_conf
                          : &ansi_default_conf);
        }
        ansi_send_text(" | ", &ansi_default_conf);
    }
}

void ansi_print_i2c_error(HAL_StatusTypeDef status, I2C_HandleTypeDef* hi2c) {
    static const struct {
        uint32_t flag;
        const char* message;
    } errors[] = {{HAL_I2C_ERROR_AF, "NACK"},
                  {HAL_I2C_ERROR_BERR, "Bus Error"},
                  {HAL_I2C_ERROR_ARLO, "Arbitration Lost"},
                  {HAL_I2C_ERROR_OVR, "Overrun/Underrun"},
                  {HAL_I2C_ERROR_TIMEOUT, "Timeout"}};

    ansi_clear_line(15, 1);
    ansi_set_cursor(15, TERMINAL_CENTER - 10);

    if (status == HAL_OK) {
        ansi_send_text("I2C OK", &ansi_default_conf);
        return;
    }

    uint32_t error = HAL_I2C_GetError(hi2c);
    char error_buff[I2C_BUFFER_SIZE] = {0};

    for (uint8_t i = 0; i < sizeof(errors) / sizeof(errors[0]); i++) {
        if (error & errors[i].flag) {
            strcat(error_buff, errors[i].message);
            strcat(error_buff, " | ");
            error &= ~errors[i].flag;
        }
    }

    if (strlen(error_buff) > 3) {
        error_buff[strlen(error_buff) - 3] = '\0';
    }

    if (error) {
        strcat(error_buff, "Unknown Error");
    }

    ansi_send_text(error_buff, &ansi_red_bold_conf);
}

void ansi_i2c_render_read_vals(i2c_perif_t* i2c_perif) {
    char buff[6];
    ansi_set_cursor(12, TERMINAL_CENTER - (2 * i2c_perif->bytes_to_catch));
    for (uint8_t i = 0; i < i2c_perif->bytes_to_catch; ++i) {
        snprintf(buff, sizeof(buff), "0x%02X ",
                 i2c_perif->slave_received_data[i]);
        ansi_send_text(buff, &ansi_default_conf);
    }
}

void ansi_i2c_render_scan(i2c_perif_t* i2c_perif) {
    uint8_t addresses[MAX_ADDRESSES];
    uint8_t count = 0;

    for (uint8_t i = 0; i < MAX_ADDRESSES; ++i) {
        if (HAL_I2C_IsDeviceReady(i2c_perif->hi2c, i << 1, 1, PERIF_DELAY) ==
            HAL_OK) {
            addresses[count++] = i;
        }
    }

    if (i2c_perif->address_scanned_found != count) {
        i2c_perif->address_scanned_found = count;
        dev_mode_request_frontend_change();
    }

    char scan_header[I2C_BUFFER_SIZE];
    snprintf(scan_header, sizeof(scan_header), "Addresses found: %d", count);
    ansi_set_cursor(11, TERMINAL_CENTER - (strlen(scan_header) / 2));
    ansi_send_text(scan_header, &ansi_default_conf);

    ansi_set_cursor(12, TERMINAL_CENTER - (2 * (count % 10)));
    char buff[6];
    for (uint8_t i = 0; i < count; ++i) {
        snprintf(buff, sizeof(buff), "0x%02X ", addresses[i]);
        ansi_send_text(buff, &ansi_default_conf);
        if ((i + 1) % 10 == 0)
            ansi_send_string("\r\n     ");
    }
}

void help_slave_i2c(void) {
    if (global_var.i2c_perif->edit_settings) {
        ansi_print_help_msg("T: stop edit | 0-F: edit slave address", 1);
        ansi_print_help_msg("X: delete adress | Y: change bit count", 0);
    } else {
        ansi_print_help_msg("T: edit settings | M: change mode", 0);
    }
}

void help_master_i2c(void) {
    if (global_var.i2c_perif->edit_settings) {
        ansi_print_help_msg("T: stop edit | 0-F: edit slave address", 1);
        ansi_print_help_msg(
            "X: delete address | Y: change bit count | U: read/write bit", 0);

    } else if (global_var.i2c_perif->edit_vals) {
        ansi_print_help_msg(
            "0-F: change val | X: delete val | L: move cursor | K: stop edit",
            0);
    } else {
        ansi_print_help_msg(
            "S: send | T: edit settings | K: edit vals  | M: change mode", 0);
    }
}

void help_display_i2c(void) {
    if (global_var.i2c_perif->edit_settings) {
        ansi_print_help_msg(
            "T: stop edit | 0-F: edit slave address | X: delete address", 0);

    } else {
        ansi_print_help_msg("S: send | T: edit settings | M: change mode", 0);
    }
}

void help_scan_i2c(void) {
    ansi_print_help_msg("M: change mode", 0);
}

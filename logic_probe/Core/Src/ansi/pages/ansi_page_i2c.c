#include "ansi_page_i2c.h"
#include "advanced/i2c.h"
#include "ansi_abstraction_layer.h"
#include "ansi_page_voltage_measure.h"
#include "global_vars.h"
#include "loop.h"
#include "stm32g0xx_hal_def.h"

extern global_vars_t global_var;

void ansi_render_i2c_measure_page(void) {
    ansi_render_border('&', "&", "");
    ansi_set_cursor(5, ADC_MEASURE_CENTER - 6);
    ansi_send_text("SCL - PB8 | SDA - PB9", &ansi_bold_conf);
    if (global_var.device_state == DEV_STATE_ADV_I2C_SLAVE) {
        ansi_i2c_render_settings(global_var.i2c_perif);
        ansi_set_cursor(4, ADC_MEASURE_CENTER);
        ansi_send_text("I2C SLAVE", &ansi_bold_conf);
        ansi_i2c_render_read_vals(global_var.i2c_perif);
    } else if (global_var.device_state == DEV_STATE_ADV_I2C_MASTER) {
        ansi_set_cursor(4, ADC_MEASURE_CENTER);
        ansi_send_text("I2C MASTER", &ansi_bold_conf);
        ansi_i2c_master_render_settings(global_var.i2c_perif);
        ansi_i2c_master_vals(global_var.i2c_perif);
    } else if (global_var.device_state == DEV_STATE_ADV_I2C_SCAN) {
        ansi_set_cursor(4, ADC_MEASURE_CENTER);
        ansi_send_text("I2C SCAN ADDRESS", &ansi_bold_conf);
        ansi_i2c_render_scan(global_var.i2c_perif);

    } else if (global_var.device_state == DEV_STATE_ADV_I2C_TEST_DISPLAY) {
    }
}

void ansi_i2c_render_settings(i2c_perif_t* i2c_perif) {
    ansi_set_cursor(6, ADC_MEASURE_CENTER - 12);
    char buff[50];
    snprintf(buff, 50, "Slave address: 0x%02X | Bytes to receive: %2d",
             i2c_perif->slave_address, i2c_perif->bytes_to_catch);
    ansi_send_text(buff, &ansi_default_conf);
    if (i2c_perif->edit_settings) {
        ansi_set_cursor(7, ADC_MEASURE_CENTER);
        ansi_send_text("Editing I2C settings!", &ansi_red_bold_conf);
    }
}

void ansi_i2c_master_render_settings(i2c_perif_t* i2c_perif) {
    ansi_set_cursor(6, ADC_MEASURE_CENTER - 12);
    char buff[50];
    snprintf(buff, 50, "Address: 0x%02X | %s | Bytes to %s: %2d",
             i2c_perif->slave_address, (i2c_perif->read_bit) ? "read" : "write",
             (i2c_perif->read_bit) ? "read" : "write",
             i2c_perif->bytes_to_catch);
    ansi_send_text(buff, &ansi_default_conf);
    if (i2c_perif->edit_settings) {
        ansi_set_cursor(7, ADC_MEASURE_CENTER);
        ansi_send_text("Editing I2C settings!", &ansi_red_bold_conf);
    }
}

void ansi_i2c_master_vals(i2c_perif_t* i2c_perif) {
    char buff[20];
    ansi_set_cursor(8, ADC_MEASURE_CENTER - (2 * i2c_perif->bytes_to_catch));
    ansi_text_config_t conf = {"", GREEN_BG, ""};
    if (i2c_perif->read_bit) {
    } else {
        for (uint8_t i = 0; i < i2c_perif->bytes_to_catch; ++i) {
            ansi_send_text(" | ", &ansi_default_conf);
            snprintf(buff, 20, "0x%02X", i2c_perif->slave_received_data[i]);
            if (i2c_perif->edit_vals && i == i2c_perif->master_index) {
                ansi_send_text(buff, &conf);
            } else {
                ansi_send_text(buff, &ansi_default_conf);
            }
        }
        ansi_send_text(" |", &ansi_default_conf);
    }
}

void ansi_i2c_warning_message(i2c_perif_t* i2c_perif) {
    ansi_clear_line(15, 1);
    if (i2c_perif->send_status == I2C_SEND_SUCCESS) {
        ansi_set_cursor(15, ADC_MEASURE_CENTER - 10);
        ansi_send_text("Success Send!", &ansi_green_bold_conf);
    } else if (i2c_perif->send_status == I2C_ERROR) {
        ansi_set_cursor(15, ADC_MEASURE_CENTER - 10);
        ansi_send_text("I2C ERROR!", &ansi_red_bold_conf);
    }
}

void ansi_i2c_render_read_vals(i2c_perif_t* i2c_perif) {
    ansi_set_cursor(12, ADC_MEASURE_CENTER - (2 * i2c_perif->bytes_to_catch));
    char buff[11];
    for (uint8_t i = 0; i < i2c_perif->bytes_to_catch; ++i) {
        snprintf(buff, 11, "0x%02X ", i2c_perif->slave_received_data[i]);
        ansi_send_text(buff, &ansi_default_conf);
    }
}

void ansi_i2c_render_scan(i2c_perif_t* i2c_perif) {
    ansi_set_cursor(11, ADC_MEASURE_CENTER);
    ansi_send_text("Addresses found:", &ansi_default_conf);
    uint8_t valid_address_counter = 0;
    uint8_t address[127];
    char buff[10];
    for (uint8_t i = 0; i < 127; ++i) {
        if (HAL_I2C_IsDeviceReady(i2c_perif->hi2c, i << 1, 1, HAL_MAX_DELAY) ==
            HAL_OK) {
            address[valid_address_counter++] = i;
        }
    }

    if (i2c_perif->address_scanned_found != valid_address_counter) {
        i2c_perif->address_scanned_found = valid_address_counter;
        dev_mode_request_frontend_change();
    }

    ansi_set_cursor(12,
                    ADC_MEASURE_CENTER - (2 * (valid_address_counter % 10)));
    for (uint8_t i = 0, cnt = 0; i < valid_address_counter; ++i, ++cnt) {
        snprintf(buff, 10, "0x%02X ", address[i]);
        ansi_send_text(buff, &ansi_default_conf);

        if (cnt == 9) {
            cnt = 0xFF;
            ansi_send_string("\r\n     ");
        }
    }
}

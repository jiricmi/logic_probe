#include "ansi_page_i2c.h"
#include "advanced/i2c.h"
#include "ansi_abstraction_layer.h"
#include "ansi_page_voltage_measure.h"
#include "global_vars.h"

extern global_vars_t global_var;

void ansi_render_i2c_measure_page(void) {
    ansi_render_border('&', "&", "");
    ansi_i2c_render_settings(global_var.i2c_perif);
    ansi_set_cursor(4, ADC_MEASURE_CENTER);
    if (global_var.device_state == DEV_STATE_ADV_I2C_SLAVE) {
        ansi_send_text("UART SLAVE", &ansi_bold_conf);
        ansi_set_cursor(5, ADC_MEASURE_CENTER - 6);
        ansi_send_text("SCL - PB8 | SDA - PB9", &ansi_bold_conf);
        ansi_i2c_render_read_vals(global_var.i2c_perif);
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

void ansi_i2c_render_read_vals(i2c_perif_t* i2c_perif) {
    ansi_set_cursor(12, ADC_MEASURE_CENTER - (2 * i2c_perif->bytes_to_catch));
    for (uint8_t i = 0; i < i2c_perif->bytes_to_catch; ++i) {
        char buff[11];
        snprintf(buff, 11, "0x%02X ", i2c_perif->slave_received_data[i]);
        ansi_send_text(buff, &ansi_default_conf);
    }
}

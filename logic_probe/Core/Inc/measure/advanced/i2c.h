#ifndef I2C_H
#define I2C_H
#include "advanced/spi.h"
#include "main.h"

#define I2C_ARRAY_SIZE 10
#define I2C_MONITOR_SIZE (I2C_ARRAY_SIZE * 2)
typedef enum {
    I2C_NONE,
    I2C_SEND_SUCCESS,
    I2C_ERROR,
    I2C_ERROR_RECIEVE
} i2c_status_t;

typedef struct {
    I2C_HandleTypeDef* hi2c;
    uint8_t slave_address;
    uint8_t bytes_to_catch;
    uint8_t slave_received_data[I2C_ARRAY_SIZE];
    uint8_t master_read_send_data[1];
    uint8_t slave_received_data_index;
    uint8_t address_scanned_found;
    _Bool edit_settings;
    _Bool edit_vals;
    uint8_t master_index;
    i2c_status_t send_status;
    _Bool read_bit;
    _Bool send_data;
    uint16_t monitor_data[I2C_ARRAY_SIZE];
} i2c_perif_t;

void i2c_init_struct(i2c_perif_t* i2c_perif, I2C_HandleTypeDef* hi2c);
void i2c_deinit_perif(i2c_perif_t* i2c_perif);

void i2c_init_perif(i2c_perif_t* i2c_perif);

void i2c_start_slave_listen(i2c_perif_t* i2c_perif);

void ansi_i2c_master_vals(i2c_perif_t* i2c_perif);

void ansi_i2c_master_render_settings(i2c_perif_t* i2c_perif);
void i2c_transmit_master(i2c_perif_t* perif);

void i2c_read_data_master(i2c_perif_t* perif);
void i2c_test_display(i2c_perif_t* perif);
void i2c_monitor_init(i2c_perif_t* i2c_perif, const spi_perif_t* spi_perif);
void i2c_monitor_deinit(spi_perif_t* spi_perif);
#endif

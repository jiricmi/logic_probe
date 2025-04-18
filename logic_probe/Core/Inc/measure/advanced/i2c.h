#ifndef I2C_H
#define I2C_H
#include "main.h"

#define I2C_ARRAY_SIZE 10

typedef struct {
    I2C_HandleTypeDef* hi2c;
    uint8_t slave_address;
    uint8_t bytes_to_catch;
    uint8_t slave_received_data[I2C_ARRAY_SIZE];
    uint8_t slave_received_data_index;
    uint8_t address_scanned_found;
    _Bool error;
    _Bool edit_settings;
} i2c_perif_t;

void i2c_init_struct(i2c_perif_t* i2c_perif, I2C_HandleTypeDef* hi2c);
void i2c_deinit_perif(i2c_perif_t* i2c_perif);

void i2c_init_perif(i2c_perif_t* i2c_perif);

void i2c_start_slave_listen(i2c_perif_t* i2c_perif);
#endif

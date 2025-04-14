#ifndef I2C_H
#define I2C_H
#include "main.h"

typedef struct {
    I2C_HandleTypeDef* hi2c;
    uint8_t slave_address;
} i2c_perif_t;

void i2c_init_struct(i2c_perif_t* i2c_perif);

#endif

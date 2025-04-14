#include "advanced/i2c.h"
#include <string.h>

void i2c_init_struct(i2c_perif_t* i2c_perif) {
    memset(i2c_perif, 0, sizeof(*i2c_perif));
}

void i2c_init_perif(i2c_perif_t* i2c_perif) {
    i2c_perif->hi2c->Instance = I2C1;
    i2c_perif->hi2c->Init.Timing = 0x10B17DB5;
    i2c_perif->hi2c->Init.OwnAddress1 = i2c_perif->slave_address;
    i2c_perif->hi2c->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    i2c_perif->hi2c->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    i2c_perif->hi2c->Init.OwnAddress2 = 0;
    i2c_perif->hi2c->Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    i2c_perif->hi2c->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    i2c_perif->hi2c->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(i2c_perif->hi2c) != HAL_OK) {
        Error_Handler();
    }

    /** Configure Analogue filter
     */
    if (HAL_I2CEx_ConfigAnalogFilter(i2c_perif->hi2c,
                                     I2C_ANALOGFILTER_ENABLE) != HAL_OK) {
        Error_Handler();
    }

    /** Configure Digital filter
     */
    if (HAL_I2CEx_ConfigDigitalFilter(i2c_perif->hi2c, 0) != HAL_OK) {
        Error_Handler();
    }
}

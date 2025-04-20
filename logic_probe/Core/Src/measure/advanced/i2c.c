#include "advanced/i2c.h"
#include <string.h>
#include "ansi_page_i2c.h"
#include "stm32g0xx_hal_def.h"

void i2c_init_struct(i2c_perif_t* i2c_perif, I2C_HandleTypeDef* hi2c) {
    memset(i2c_perif, 0, sizeof(*i2c_perif));
    i2c_perif->hi2c = hi2c;
    i2c_perif->slave_address = 36;
    i2c_perif->bytes_to_catch = 1;
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

void i2c_deinit_perif(i2c_perif_t* i2c_perif) {
    HAL_I2C_DisableListen_IT(i2c_perif->hi2c);
    HAL_I2C_DeInit(i2c_perif->hi2c);
}

void i2c_start_slave_listen(i2c_perif_t* i2c_perif) {
    HAL_I2C_EnableListen_IT(i2c_perif->hi2c);
}

void i2c_transmit_master(i2c_perif_t* perif) {
    if (perif->send_data) {
        HAL_StatusTypeDef ret = HAL_I2C_Master_Transmit(
            perif->hi2c, perif->slave_address << 1, perif->slave_received_data,
            perif->bytes_to_catch, HAL_MAX_DELAY);
        perif->send_data = 0;
        if (ret == HAL_OK) {
            perif->send_status = I2C_SEND_SUCCESS;
        } else {
            perif->send_status = I2C_ERROR;
        }
        ansi_print_i2c_error(ret, perif->hi2c);
    }
}

void i2c_read_data_master(i2c_perif_t* perif) {
    if (perif->send_data) {
        perif->send_data = 0;
        HAL_StatusTypeDef ret = HAL_I2C_Master_Transmit(
            perif->hi2c, perif->slave_address << 1,
            perif->master_read_send_data, 1, HAL_MAX_DELAY);

        if (ret == HAL_OK) {
            ret = HAL_I2C_Master_Receive(perif->hi2c, perif->slave_address << 1,
                                         perif->slave_received_data,
                                         perif->bytes_to_catch, HAL_MAX_DELAY);
            if (ret == HAL_OK) {
                perif->send_status = I2C_SEND_SUCCESS;
            } else {
                perif->send_status = I2C_ERROR_RECIEVE;
            }
        } else {
            perif->send_status = I2C_ERROR;  // TODO: TADY PROBLEM
        }
        ansi_print_i2c_error(ret, perif->hi2c);
    }
}

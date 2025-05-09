#include <stdlib.h>
#include <string.h>
#include "advanced/i2c.h"
#include "advanced/spi.h"
#include "ansi_abstraction_layer.h"

uint8_t init_sequence[] = {0xAE, 0xD5, 0x80, 0xA8, 0x3F, 0xD3, 0x00, 0x40, 0x8D,
                           0x14, 0x20, 0x02, 0xA0, 0xC0, 0xDA, 0x12, 0x81, 0xCF,
                           0xD9, 0xF1, 0xDB, 0x40, 0xA5, 0xA6, 0xAF};

HAL_StatusTypeDef SSD1306_i2c_write_data(i2c_perif_t* perif,
                                         uint8_t* data,
                                         uint16_t size) {
    uint8_t* buffer = malloc(size + 1);
    if (!buffer)
        return HAL_ERROR;

    buffer[0] = 0x40;
    memcpy(buffer + 1, data, size);

    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(
        perif->hi2c, perif->slave_address << 1, buffer, size + 1, 100);

    free(buffer);
    return status;
}

HAL_StatusTypeDef SSD1306_i2c_write_command(i2c_perif_t* perif,
                                            uint8_t command) {
    uint8_t data[2] = {0x00, command};
    return HAL_I2C_Master_Transmit(perif->hi2c, perif->slave_address << 1, data,
                                   sizeof(data), 100);
}

HAL_StatusTypeDef SSD1306_update_display(i2c_perif_t* perif,
                                         uint8_t* buffer,
                                         size_t size) {
    HAL_StatusTypeDef status;

    if ((status = SSD1306_i2c_write_command(perif, 0x21)))
        return status;
    if ((status = SSD1306_i2c_write_command(perif, 0x00)))
        return status;
    if ((status = SSD1306_i2c_write_command(perif, 0x7F)))
        return status;
    if ((status = SSD1306_i2c_write_command(perif, 0x22)))
        return status;
    if ((status = SSD1306_i2c_write_command(perif, 0x00)))
        return status;
    if ((status = SSD1306_i2c_write_command(perif, 0x07)))
        return status;

    return SSD1306_i2c_write_data(perif, buffer, size);
}

HAL_StatusTypeDef SSD1306_i2c_init_display(i2c_perif_t* perif) {
    HAL_StatusTypeDef status;

    for (uint8_t i = 0; i < sizeof(init_sequence); i++) {
        if ((status = SSD1306_i2c_write_command(perif, init_sequence[i]))) {
            return status;
        }
    }

    return HAL_OK;
}

HAL_StatusTypeDef SSD_i2c_test_display(i2c_perif_t* perif) {
    HAL_StatusTypeDef status;

    if ((status = SSD1306_i2c_init_display(perif)))
        return status;

    const uint8_t test_sequence[] = {0xA5, 0xA6, 0xAF};
    for (uint8_t i = 0; i < sizeof(test_sequence); i++) {
        if ((status = SSD1306_i2c_write_command(perif, test_sequence[i]))) {
            return status;
        }
    }

    return HAL_OK;
}

void SSD1306_spi_low(void) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
}
void SSD1306_spi_high(void) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
}

HAL_StatusTypeDef SSD1306_spi_write_command(SPI_HandleTypeDef* hspi,
                                            uint8_t command) {
    HAL_StatusTypeDef status = HAL_SPI_Transmit(hspi, &command, 1, 100);
    return status;
}

HAL_StatusTypeDef SSD1306_spi_write_data(SPI_HandleTypeDef* hspi,
                                         uint8_t* data,
                                         uint16_t size) {
    HAL_StatusTypeDef status = HAL_SPI_Transmit(hspi, data, size, 100);
    return status;
}

HAL_StatusTypeDef SSD1306_spi_update_display(SPI_HandleTypeDef* hspi,
                                             uint8_t* buffer,
                                             size_t size) {
    HAL_StatusTypeDef status;

    if ((status = SSD1306_spi_write_command(hspi, 0x21)))
        return status;
    if ((status = SSD1306_spi_write_command(hspi, 0x00)))
        return status;
    if ((status = SSD1306_spi_write_command(hspi, 0x7F)))
        return status;

    if ((status = SSD1306_spi_write_command(hspi, 0x22)))
        return status;
    if ((status = SSD1306_spi_write_command(hspi, 0x00)))
        return status;
    if ((status = SSD1306_spi_write_command(hspi, 0x07)))
        return status;

    return SSD1306_spi_write_data(hspi, buffer, size);
}

HAL_StatusTypeDef SSD1306_spi_init_display(SPI_HandleTypeDef* hspi) {
    HAL_StatusTypeDef status;

    for (uint8_t i = 0; i < sizeof(init_sequence); i++) {
        if ((status = SSD1306_spi_write_command(hspi, init_sequence[i]))) {
            return status;
        }
        HAL_Delay(40);
    }
    return HAL_OK;
}

void SSD1306_spi_test_display(spi_perif_t* perif) {
    HAL_StatusTypeDef status;
    SSD1306_spi_low();

    status = SSD1306_spi_init_display(perif->hspi);

    if (status != HAL_OK) {
        perif->error = SPI_ERROR_SEND;
        SSD1306_spi_high();
        return;
    }

    const uint8_t test_sequence[] = {0xA5, 0xA6, 0xAF};
    for (uint8_t i = 0; i < sizeof(test_sequence); i++) {
        status = SSD1306_spi_write_command(perif->hspi, test_sequence[i]);
        HAL_Delay(40);

        if (status != HAL_OK) {
            perif->error = SPI_ERROR_SEND;
            SSD1306_spi_high();
            return;
        }
    }

    perif->error = SPI_ERROR_SUCCESS;

    SSD1306_spi_high();
}

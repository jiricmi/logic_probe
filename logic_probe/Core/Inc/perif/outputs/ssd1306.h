#ifndef SSD1306_H
#define SSD1306_H
#include <stdint.h>
#include <string.h>
#include "advanced/i2c.h"
#include "advanced/spi.h"

HAL_StatusTypeDef SSD1306_i2c_write_data(i2c_perif_t* perif,
                                         uint8_t* data,
                                         uint16_t size);
HAL_StatusTypeDef SSD1306_i2c_write_command(i2c_perif_t* perif,
                                            uint8_t command);
HAL_StatusTypeDef SSD1306_i2c_update_display(i2c_perif_t* perif,
                                             uint8_t* buffer,
                                             size_t size);
HAL_StatusTypeDef SSD1306_i2c_init_display(i2c_perif_t* perif);
HAL_StatusTypeDef SSD_i2c_test_display(i2c_perif_t* perif);

void SSD1306_spi_cs_low(void);

void SSD1306_spi_cs_high(void);

HAL_StatusTypeDef SSD1306_spi_write_command(SPI_HandleTypeDef* hspi,
                                            uint8_t command);

HAL_StatusTypeDef SSD1306_spi_write_data(SPI_HandleTypeDef* hspi,
                                         uint8_t* data,
                                         uint16_t size);

HAL_StatusTypeDef SSD1306_spi_update_display(SPI_HandleTypeDef* hspi,
                                             uint8_t* buffer,
                                             size_t size);

HAL_StatusTypeDef SSD1306_spi_init_display(SPI_HandleTypeDef* hspi);

void SSD1306_spi_test_display(spi_perif_t* perif);
#endif

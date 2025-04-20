#ifndef SSD1306_H
#define SSD1306_H
#include <stdint.h>
#include <string.h>
#include "advanced/i2c.h"

HAL_StatusTypeDef SSD1306_write_data(i2c_perif_t* perif,
                                     uint8_t* data,
                                     uint16_t size);
HAL_StatusTypeDef SSD1306_write_command(i2c_perif_t* perif, uint8_t command);
HAL_StatusTypeDef SSD1306_update_display(i2c_perif_t* perif,
                                         uint8_t* buffer,
                                         size_t size);
HAL_StatusTypeDef SSD1306_init_display(i2c_perif_t* perif);
HAL_StatusTypeDef SSD_test_display(i2c_perif_t* perif);
#endif

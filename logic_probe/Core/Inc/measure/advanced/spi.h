#ifndef SPI_H
#define SPI_H
#include <stdint.h>
#include "main.h"

typedef struct {
    SPI_HandleTypeDef* hspi;
    _Bool slave_mode;
    _Bool CLK_polarity;
    _Bool msb;
    _Bool CLK_phase;

} spi_perif_t;

void spi_init_struct(spi_perif_t* perif, SPI_HandleTypeDef* hspi);

void spi_deinit_perif(spi_perif_t* perif);

void spi_init_perif(spi_perif_t* perif);
#endif

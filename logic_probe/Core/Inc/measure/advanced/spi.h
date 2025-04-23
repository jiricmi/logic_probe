#ifndef SPI_H
#define SPI_H
#include <stdint.h>
#include "main.h"

#define SPI_ARRAY_SIZE 10
typedef enum {
    SPI_ERROR_NONE,
    SPI_ERROR_RECEIVE,
    SPI_ERROR_SEND,
} spi_error_t;

typedef struct {
    SPI_HandleTypeDef* hspi;
    _Bool slave_mode;
    _Bool CLK_polarity;
    _Bool msb;
    _Bool CLK_phase;
    _Bool edit_settings;
    _Bool read_bit;
    _Bool edit_vals;
    _Bool send_data;
    spi_error_t error;
    uint8_t master_index;
    uint8_t master_send_data[1];
    uint8_t bytes_count;
    uint8_t data[SPI_ARRAY_SIZE];

} spi_perif_t;

void spi_init_struct(spi_perif_t* perif, SPI_HandleTypeDef* hspi);

void spi_deinit_perif(spi_perif_t* perif);

void spi_init_perif(spi_perif_t* perif);

void ansi_spi_master_vals(spi_perif_t* perif);

void ansi_spi_render_slave_settings(spi_perif_t* perif);

void spi_transmit(spi_perif_t* perif);
#endif

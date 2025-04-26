#ifndef ANSI_PAGE_SPI_H
#define ANSI_PAGE_SPI_H
#include "advanced/spi.h"

void ansi_render_spi_measure_page(void);

void ansi_spi_render_master_settings(spi_perif_t* perif);

void ansi_spi_render_read_vals(spi_perif_t* perif);

void ansi_spi_print_error(spi_perif_t* perif);

void ansi_spi_test_display_render_settings(void);

void help_slave_spi(void);

void help_master_spi(void);

void help_spi_display(void);
#endif

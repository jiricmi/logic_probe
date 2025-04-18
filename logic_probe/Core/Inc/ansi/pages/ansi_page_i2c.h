#ifndef ANSI_PAGE_I2C_H
#define ANSI_PAGE_I2C_H

#include "advanced/i2c.h"
void ansi_render_i2c_measure_page(void);

void ansi_i2c_render_settings(i2c_perif_t* i2c_perif);

void ansi_i2c_render_read_vals(i2c_perif_t* i2c_perif);

void ansi_i2c_render_scan(i2c_perif_t* i2c_perif);
#endif

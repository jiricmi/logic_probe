#ifndef ANSI_PAGES_NEOPIXEL_MEASURE_H
#define ANSI_PAGES_NEOPIXEL_MEASURE_H

#include "advanced/neopixel.h"
void ansi_render_neopixel_measure_page(void);

void ansi_render_neopixel_measure_vals(neopixel_measure_t* data);

void ansi_render_neopixel_send_vals(neopixel_measure_t* data);

void help_neopixel(void);
#endif

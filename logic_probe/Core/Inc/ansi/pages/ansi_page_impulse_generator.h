#ifndef ANSI_PAGE_IMPULSE_GENERATOR_H
#define ANSI_PAGE_IMPULSE_GENERATOR_H

#include "signal_generator.h"

void ansi_render_impulse_generator_page(void);

void ansi_render_impulse_generator(const sig_generator_t* generator);

void ansi_impulse_generator_render_hint(void);
#endif

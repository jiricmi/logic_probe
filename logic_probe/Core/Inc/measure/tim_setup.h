#ifndef TIM_SETUP_H
#define TIM_SETUP_H

#include "advanced/neopixel.h"
#include "signal_detector.h"
#include "signal_generator.h"

void detector_slave_init_pulse_width(sig_detector_t* detector);

void detector_slave_init_frequency(sig_detector_t* detector);

void sig_gen_init_timer(sig_generator_t* generator, _Bool polarity);

void adv_neopixel_read_init_timers(neopixel_measure_t* data);
#endif

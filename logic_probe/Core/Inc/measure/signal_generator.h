#ifndef SIGNAL_GENERATOR_H
#define SIGNAL_GENERATOR_H

#include <stdint.h>
typedef struct {
    uint8_t mode;
    uint32_t period;
    _Bool input_state;
    _Bool start;
    _Bool con;
} sig_generator_t;

void sig_gen_init(sig_generator_t* generator);

void sig_gen_change_period(sig_generator_t* generator, const int change);

void sig_gen_toggle_pulse(sig_generator_t* generator, const _Bool con);
#endif

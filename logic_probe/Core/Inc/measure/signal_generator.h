#ifndef SIGNAL_GENERATOR_H
#define SIGNAL_GENERATOR_H

#include <stdint.h>
#include "my_bool.h"
typedef struct {
    uint8_t mode;
    uint32_t period;
    bool input_state;
    bool start;
} sig_gen_t;

void sig_gen_init(sig_gen_t* generator);

void sig_gen_change_period(sig_gen_t* generator, const int change);

void sig_gen_activate_pulse(sig_gen_t* generator);
#endif

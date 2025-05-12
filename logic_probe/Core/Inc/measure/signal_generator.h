#ifndef SIGNAL_GENERATOR_H
#define SIGNAL_GENERATOR_H

#include <stdint.h>
#include "main.h"

#define MIN_PERIOD_VALUE 2
#define N_OF_OUTPUTS 3

#define GEN_PINS_ARRAY {GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3}

typedef enum {
    SIG_GEN_MODE_PULSE_UP,
    SIG_GEN_MODE_PULSE_DOWN,
} sig_gen_mode_t;

typedef struct {
    TIM_HandleTypeDef* htim;
    sig_gen_mode_t mode;
    uint32_t period_us;
    uint32_t period_us_temp;
    _Bool edit_period;
    _Bool edit_repeat;
    _Bool channel_state[N_OF_OUTPUTS];
    uint8_t repeat;
    _Bool local_pulsing;
    _Bool send;
    _Bool permanent_send;
} sig_generator_t;

void init_generator(sig_generator_t* generator, TIM_HandleTypeDef* htim);

void generator_change_mode(sig_generator_t* generator);

void generator_toggle_signal(sig_generator_t* generator, const uint8_t n);
void generator_toggle_all_signal_off(void);
void generator_apply_temp_period(sig_generator_t* generator);
void generator_unapply_temp_period(sig_generator_t* generator);
void generator_setup_timers(sig_generator_t* generator);
void generator_send_pulse(sig_generator_t* generator);
void generator_send_permanent(sig_generator_t* generator);

#endif

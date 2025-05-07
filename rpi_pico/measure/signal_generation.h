#ifndef SIGNAL_GENERATION_H
#define SIGNAL_GENERATION_H

#include "hardware/pio.h"
#include "pico/stdlib.h"

#define GEN_PIN 17
#define PWM_PIN 16

typedef enum {
    SIG_GEN_MODE_PULSE_UP,
    SIG_GEN_MODE_PULSE_DOWN

} sig_gen_mode_t;

typedef struct {
    repeating_timer_t timer;
    sig_gen_mode_t mode;
    PIO pio;
    uint pio_offset;
    uint sm;
    uint32_t period_us;
    bool edit_period;
    bool edit_pwm;
    uint32_t pwm_duty;
    uint32_t pwm_period;
    bool pwm_send;
    bool send;
    bool enable_pwm;
    bool pwm_state;
} sig_gen_t;

void sig_gen_init_struct(sig_gen_t* perif);
void sig_gen_send_perif(sig_gen_t* perif);
void sig_gen_deinit_perif(sig_gen_t* perif);
void sig_gen_start_pwm(sig_gen_t* perif);
void sig_gen_stop_pwm(sig_gen_t* perif);
#endif

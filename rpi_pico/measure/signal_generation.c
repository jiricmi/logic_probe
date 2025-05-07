#include "signal_generation.h"
#include <pico/time.h>
#include <stdio.h>
#include "global_vars.h"
#include "hardware/pio.h"
#include "hardware/pwm.h"
#include "pulse_down.pio.h"
#include "pulse_up.pio.h"

extern global_vars_t global_var;

void sig_gen_init_struct(sig_gen_t* perif) {
    perif->period_us = 1000;
    perif->pwm_period = 500000;
    perif->pwm_duty = 50;
}

void sig_gen_send_perif(sig_gen_t* perif) {
    sig_gen_deinit_perif(perif);
    perif->pio = pio0;
    if (perif->mode == SIG_GEN_MODE_PULSE_UP) {
        perif->pio_offset = pio_add_program(perif->pio, &pulse_up_program);
    } else {
        perif->pio_offset = pio_add_program(perif->pio, &pulse_down_program);
    }

    perif->sm = pio_claim_unused_sm(perif->pio, true);
    if (perif->mode == SIG_GEN_MODE_PULSE_UP) {
        pulse_up_program_init(perif->pio, perif->sm, perif->pio_offset,
                              GEN_PIN);
    } else {
        pulse_down_program_init(perif->pio, perif->sm, perif->pio_offset,
                                GEN_PIN);
    }

    pio_sm_put(perif->pio, perif->sm, perif->period_us);
    pio_sm_set_enabled(perif->pio, perif->sm, true);
}

void sig_gen_deinit_perif(sig_gen_t* perif) {
    if (pio_sm_is_claimed(perif->pio, perif->sm)) {
        pio_sm_set_enabled(perif->pio, perif->sm, false);
        pio_sm_unclaim(perif->pio, perif->sm);
    }
}

bool PWM_callback(repeating_timer_t* rt) {
    gpio_put(PWM_PIN, global_var.sig_gen_perif.pwm_state);
    global_var.sig_gen_perif.pwm_state = !global_var.sig_gen_perif.pwm_state;
    return true;
}

void sig_gen_start_pwm(sig_gen_t* perif) {
    if (!add_repeating_timer_ms(perif->pwm_period / 2, PWM_callback, NULL,
                                &perif->timer)) {
        printf("Error timer pwm");
    }
}

void sig_gen_stop_pwm(sig_gen_t* perif) {
    cancel_repeating_timer(&perif->timer);
}

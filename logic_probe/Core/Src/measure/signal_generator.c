#include "signal_generator.h"
#include <stdbool.h>
#include <string.h>
#include "stm32g0xx_hal_gpio.h"
#include "tim_setup.h"

extern TIM_HandleTypeDef htim16;

void init_generator(sig_generator_t* generator, TIM_HandleTypeDef* htim) {
    memset(generator, 0, sizeof(*generator));

    generator->mode = SIG_GEN_MODE_PULSE_UP;
    generator->period_us = 2000;
    generator->period_us_temp = generator->period_us;
    generator->htim = htim;
    generator->edit_period = false;
}

void generator_change_mode(sig_generator_t* generator) {
    if (generator->mode == SIG_GEN_MODE_PULSE_DOWN) {
        generator->mode = SIG_GEN_MODE_PULSE_UP;
    } else {
        generator->mode = SIG_GEN_MODE_PULSE_DOWN;
    }
}

void generator_toggle_signal(sig_generator_t* generator, const uint8_t n) {
    if (n < 1 || n > N_OF_OUTPUTS) {
        return;
    }

    const uint16_t pins[] = GEN_PINS_ARRAY;

    generator->channel_state[n - 1] = !generator->channel_state[n - 1];
    uint16_t pin = pins[n - 1];
    GPIO_PinState state =
        (generator->channel_state[n - 1]) ? GPIO_PIN_SET : GPIO_PIN_RESET;

    HAL_GPIO_WritePin(GPIOA, pin, state);
}

void generator_toggle_all_signal_off(void) {
    const uint16_t pins[] = GEN_PINS_ARRAY;

    for (uint8_t i = 0; i < N_OF_OUTPUTS; ++i) {
        HAL_GPIO_WritePin(GPIOA, pins[i], GPIO_PIN_RESET);
    }
}

void generator_apply_temp_period(sig_generator_t* generator) {
    if (generator->period_us_temp < MIN_PERIOD_VALUE) {
        generator->period_us_temp = MIN_PERIOD_VALUE;
    }

    generator->period_us = generator->period_us_temp;
    generator->edit_period = false;
}

void generator_unapply_temp_period(sig_generator_t* generator) {
    generator->period_us_temp = generator->period_us;
    generator->edit_period = false;
}

void generator_setup_timers(sig_generator_t* generator) {
    HAL_TIM_PWM_Stop(generator->htim, TIM_CHANNEL_1);
    switch (generator->mode) {
        case SIG_GEN_MODE_PULSE_UP:
            sig_gen_init_timer(generator, true);
            break;
        case SIG_GEN_MODE_PULSE_DOWN:
            sig_gen_init_timer(generator, false);
            break;
    }
    HAL_TIM_PWM_Start(generator->htim, TIM_CHANNEL_1);
}

void generator_send_pulse(sig_generator_t* generator) {
    HAL_TIM_PWM_Stop(generator->htim, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(generator->htim, TIM_CHANNEL_1);
}

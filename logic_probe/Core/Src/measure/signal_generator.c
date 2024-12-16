#include "signal_generator.h"
#include "main.h"

extern TIM_HandleTypeDef htim16;

void sig_gen_init(sig_gen_t* generator) {
    generator->mode = 0;
    generator->period = 1000;
    generator->input_state = false;
    generator->start = false;
    generator->con = false;
}

void sig_gen_change_period(sig_gen_t* generator, const int change) {
    generator->period += change;

    if (generator->period <= 2) {
        generator->period = 2;
    } else if (generator->period >= 1000) {
        generator->period = 1000;
    }
}

void sig_gen_toggle_pulse(sig_gen_t* generator, const bool con) {
    generator->start = false;
    __HAL_TIM_SET_AUTORELOAD(&htim16, generator->period - 1);

    if (con && generator->con) {
        HAL_TIM_Base_Stop_IT(&htim16);
        generator->con = false;
    } else {
        generator->con = con;
    }

    HAL_TIM_Base_Start_IT(&htim16);
}

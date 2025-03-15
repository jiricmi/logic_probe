#include "signal_detector.h"
#include <stdbool.h>
#include <string.h>
#include "global_vars.h"
#include "main.h"
#include "tim_setup.h"
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim14;
extern TIM_HandleTypeDef htim16;

void init_detector(sig_detector_t* detector,
                   TIM_HandleTypeDef* slave_tim,
                   TIM_HandleTypeDef* master_tim) {
    memset(detector, 0, sizeof(*detector));
    detector->slave_tim = slave_tim;
    detector->master_tim = master_tim;
    detector->slave_tim_mode = SLAVE_MODE_FREQUENCY;
    detector->mode = DETECTOR_MODE_FREQUENCY;
    detector->sample_time_index = DETECTOR_BASE_SAMPLE_TIME;
}

void detector_change_sample_time(sig_detector_t* detector) {
    if (detector->sample_time_index == 0) {
        detector->sample_time_index = DETECTOR_SAMPLE_TIMES_COUNT - 1;
    } else {
        --detector->sample_time_index;
    }
    __HAL_TIM_SET_AUTORELOAD(&htim3,
                             SAMPLE_TIMES[detector->sample_time_index] - 1);
}

void detector_setup_timers(sig_detector_t* detector) {
    HAL_TIM_Base_Stop_IT(detector->master_tim);
    HAL_TIM_Base_Stop(detector->slave_tim);
    HAL_TIM_IC_Stop_IT(detector->slave_tim, TIM_CHANNEL_1);
    HAL_TIM_IC_Stop_IT(detector->slave_tim, TIM_CHANNEL_2);
    __HAL_TIM_SET_COUNTER(detector->master_tim, 0);
    __HAL_TIM_SET_COUNTER(detector->slave_tim, 0);

    switch (detector->mode) {
        case (DETECTOR_MODE_FREQUENCY):
            detector_slave_init_frequency(detector);
            HAL_TIM_Base_Start_IT(detector->master_tim);
            break;

        case (DETECTOR_MODE_PULSE_UP):
        case (DETECTOR_MODE_PULSE_DOWN):
            detector_slave_init_pulse_width(detector);
            HAL_TIM_IC_Start_IT(detector->slave_tim, TIM_CHANNEL_1);
            HAL_TIM_IC_Start_IT(detector->slave_tim, TIM_CHANNEL_2);
            break;
    }
}

void detector_change_mode(sig_detector_t* detector) {
    if (detector->mode != DETECTOR_MODE_FREQUENCY) {
        detector->mode = detector->mode + 1;
    } else {
        detector->mode = DETECTOR_MODE_PULSE_UP;
    }
    detector->one_pulse_found = false;
    detector_setup_timers(detector);
}

uint32_t detector_get_gated_value(uint32_t n_pulses, uint8_t index) {
    uint32_t frequency;

    if (SAMPLE_TIMES[index] < 1000) {
        frequency = n_pulses * SAMPLE_TIMES_1000[index];
    } else {
        frequency = n_pulses / SAMPLE_TIMES_1000[index];
    }
    return frequency;
}

void detector_compute_freq_measures(sig_detector_t* detector) {
    uint32_t* edge_times = detector->edge_times;

    uint32_t high_delta =
        edge_times[DET_EDGE2_FALL] - edge_times[DET_EDGE1_RISE];

    uint32_t low_delta =
        edge_times[DET_EDGE3_RISE] - edge_times[DET_EDGE2_FALL];

    detector->pwm_duty = (high_delta * 1000) / (edge_times[DET_EDGE3_RISE] -
                                                edge_times[DET_EDGE1_RISE]);

    detector->widths[DET_LOW_WIDTH] = (low_delta * 10) / PROCESSOR_FREQ_IN_MHZ;
    detector->widths[DET_HIGH_WIDTH] =
        (high_delta * 10) / PROCESSOR_FREQ_IN_MHZ;

    if (detector->frequency == 0) {
        detector->widths[DET_HIGH_WIDTH] = 0;
        detector->widths[DET_LOW_WIDTH] = 0;
    }
}

void detector_parse_catched_signal(sig_detector_t* detector) {
    TIM_HandleTypeDef* htim = detector->slave_tim;
    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
        detector_check_signal_raise_edge(detector);
    } else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2) {
        detector_check_signal_fall_edge(detector);
    }
}

void detector_check_signal_raise_edge(sig_detector_t* detector) {
    if (!detector->edge_catch[DET_EDGE1_RISE]) {
        detector->edge_times[DET_EDGE1_RISE] =
            HAL_TIM_ReadCapturedValue(detector->slave_tim, TIM_CHANNEL_1);
        detector->edge_catch[DET_EDGE1_RISE] = true;
    } else if (!detector->edge_catch[DET_EDGE3_RISE]) {
        detector->edge_times[DET_EDGE3_RISE] =
            HAL_TIM_ReadCapturedValue(detector->slave_tim, TIM_CHANNEL_1);
        detector->edge_catch[DET_EDGE3_RISE] = true;

        detector_compute_freq_measures(detector);

        HAL_TIM_IC_Stop_IT(detector->slave_tim, TIM_CHANNEL_1);
        HAL_TIM_IC_Stop_IT(detector->slave_tim, TIM_CHANNEL_2);
    }
}

void detector_check_signal_fall_edge(sig_detector_t* detector) {
    if (detector->edge_catch[DET_EDGE1_RISE] &&
        !detector->edge_catch[DET_EDGE2_FALL]) {
        detector->edge_times[DET_EDGE2_FALL] =
            HAL_TIM_ReadCapturedValue(detector->slave_tim, TIM_CHANNEL_2);
        detector->edge_catch[DET_EDGE2_FALL] = true;
    }
}

void detector_parse_pulse_catcher(sig_detector_t* detector) {
    _Bool is_channel_1 =
        detector->slave_tim->Channel == HAL_TIM_ACTIVE_CHANNEL_1;
    _Bool is_channel_2 =
        detector->slave_tim->Channel == HAL_TIM_ACTIVE_CHANNEL_2;
    _Bool is_pulse_up_mode = detector->mode == DETECTOR_MODE_PULSE_UP;
    _Bool is_pulse_down_mode = detector->mode == DETECTOR_MODE_PULSE_DOWN;

    if ((is_channel_1 && is_pulse_up_mode) ||
        (is_channel_2 && is_pulse_down_mode)) {
        detector->one_pulse_found = true;
    }
}

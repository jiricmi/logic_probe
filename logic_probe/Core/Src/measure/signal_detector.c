#include "signal_detector.h"
#include <stdbool.h>
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim14;
extern TIM_HandleTypeDef htim16;

void init_detector(sig_detector_t* detector, TIM_HandleTypeDef* htim) {
    detector->htim = htim;
    detector->high_start = 0;
    detector->high_end = 0;
    detector->high_width = 0;
    detector->high_catched = false;
    detector->high_calculated = false;
    detector->p = false;
    detector->low_start = 0;
    detector->low_end = 0;
    detector->low_width = 0;
    detector->low_catched = false;
    detector->low_calculated = false;
    detector->mode = 0;
    detector->pulse_catch_mode = 0;
    detector->active = true;
    uint16_t sample_times[] = {50, 100, 200, 500, 1000, 2000};

    for (uint8_t i = 0; i < DETECTOR_SAMPLE_TIMES_COUNT; ++i) {
        detector->sample_times[i] = sample_times[i];
    }

    detector->sample_time_index = DETECTOR_SAMPLE_TIMES_COUNT - 2;
    detector->pulse_count = 0;
    detector->frequency = 0;
}

void detector_set_to_default(sig_detector_t* detector) {
    detector->high_start = 0;
    detector->high_end = 0;
    detector->high_width = 0;
    detector->high_catched = false;
    detector->high_calculated = false;
    detector->p = false;
    detector->low_start = 0;
    detector->low_end = 0;
    detector->low_width = 0;
    detector->low_catched = false;
    detector->low_calculated = false;
}

void detector_change_sample_time(sig_detector_t* detector) {
    if (detector->sample_time_index == 0) {
        detector->sample_time_index = DETECTOR_SAMPLE_TIMES_COUNT - 1;
    } else {
        --detector->sample_time_index;
    }
    __HAL_TIM_SET_AUTORELOAD(
        &htim3, detector->sample_times[detector->sample_time_index] - 1);
}

void detector_change_mode(sig_detector_t* detector) {
    if (detector->mode < DETECTOR_MODE_COUNT - 1) {
        ++detector->mode;
    } else {
        detector->mode = 0;
    }
    detector_set_to_default(detector);
}

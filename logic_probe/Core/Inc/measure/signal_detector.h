#ifndef SIGNAL_DETECTOR_H
#define SIGNAL_DETECTOR_H

#include <stdint.h>
#include "main.h"
#include "my_bool.h"
#define DETECTOR_SAMPLE_TIMES_COUNT 6
#define DETECTOR_MODE_COUNT 3

typedef struct {
    TIM_HandleTypeDef* htim;
    uint32_t high_start;
    uint32_t high_end;
    uint32_t low_start;
    uint32_t low_end;
    uint32_t high_width;
    uint32_t low_width;
    bool p;
    bool high_catched;
    bool high_calculated;
    bool low_catched;
    bool low_calculated;
    uint8_t mode;
    uint8_t pulse_catch_mode;
    bool active;
    uint16_t sample_times[DETECTOR_SAMPLE_TIMES_COUNT];
    uint8_t sample_time_index;
    uint32_t pulse_count;
    uint32_t frequency;

} sig_detector_t;

void init_detector(sig_detector_t* detector, TIM_HandleTypeDef* htim);

void detector_change_sample_time(sig_detector_t* detector);
#endif

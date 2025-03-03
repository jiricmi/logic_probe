#ifndef SIGNAL_DETECTOR_H
#define SIGNAL_DETECTOR_H

#include <stdint.h>
#include "main.h"
#include "my_bool.h"
#define DETECTOR_SAMPLE_TIMES_COUNT 6
#define DETECTOR_MODE_COUNT 3

typedef enum {
    DETECTOR_MODE_PULSE_UP,
    DETECTOR_MODE_PULSE_DOWN,
    DETECTOR_MODE_FREQUENCY
} detector_mode_t;

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
    detector_mode_t mode;
    uint8_t pulse_catch_mode;
    bool active;
    uint16_t sample_times[DETECTOR_SAMPLE_TIMES_COUNT];
    uint8_t sample_time_index;
    uint32_t pulse_count;
    uint32_t frequency;

} sig_detector_t;

void init_detector(sig_detector_t* detector, TIM_HandleTypeDef* htim);

void detector_change_sample_time(sig_detector_t* detector);

void detector_change_mode(sig_detector_t* detector);
#endif

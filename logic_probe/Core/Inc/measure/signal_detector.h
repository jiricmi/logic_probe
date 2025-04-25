#ifndef SIGNAL_DETECTOR_H
#define SIGNAL_DETECTOR_H

#include <stdint.h>
#include "main.h"

#define DETECTOR_SAMPLE_TIMES_COUNT 8
#define DETECTOR_BASE_SAMPLE_TIME 4
#define DETECTOR_N_OF_EDGES 3
#define DETECTOR_N_OF_WIDTHS 2

typedef enum { DET_EDGE1_RISE, DET_EDGE2_FALL, DET_EDGE3_RISE } det_n_edge_t;

typedef enum { DET_LOW_WIDTH, DET_HIGH_WIDTH } det_n_width_t;

typedef enum {
    DETECTOR_MODE_PULSE_UP,
    DETECTOR_MODE_PULSE_DOWN,
    DETECTOR_MODE_FREQUENCY
} detector_mode_t;

typedef enum { SLAVE_MODE_FREQUENCY, SLAVE_MODE_WIDTH } detector_slave_mode_t;

static const uint16_t GATE_TIMES[] = {50,   100,  200,  500,
                                      1000, 2000, 5000, 10000};

static const uint16_t SAMPLE_TIMES_1000[] = {20, 10, 5, 2, 1, 2, 5, 10};

typedef struct {
    TIM_HandleTypeDef* slave_tim;
    TIM_HandleTypeDef* master_tim;
    detector_slave_mode_t slave_tim_mode;
    uint32_t frequency;
    uint32_t rec_frequency;
    detector_mode_t mode;
    uint8_t gate_time_index;
    uint32_t edge_times[DETECTOR_N_OF_EDGES];
    uint32_t widths[DETECTOR_N_OF_WIDTHS];
    _Bool edge_catch[DETECTOR_N_OF_EDGES];
    uint16_t pwm_duty;
    _Bool one_pulse_found;
} sig_detector_t;

void init_detector(sig_detector_t* detector,
                   TIM_HandleTypeDef* slave_tim,
                   TIM_HandleTypeDef* master_tim);
void detector_frequency_normalize_widths(sig_detector_t* detector);
void detector_change_sample_time(sig_detector_t* detector);
void detector_setup_timers(sig_detector_t* detector, _Bool stop_timers);
void detector_change_mode(sig_detector_t* detector);
uint32_t detector_get_gated_value(uint32_t n_pulses, uint8_t index);
void detector_compute_freq_measures(sig_detector_t* detector);
void detector_parse_catched_signal(sig_detector_t* detector);
void detector_check_signal_raise_edge(sig_detector_t* detector);
void detector_check_signal_fall_edge(sig_detector_t* detector);
void detector_parse_pulse_catcher(sig_detector_t* detector);
#endif

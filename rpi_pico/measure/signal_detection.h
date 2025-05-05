#ifndef SIGNAL_DETECTION_H
#define SIGNAL_DETECTION_H
#include <stdint.h>
#include "pico/stdlib.h"

#define FREQUECY_PIN 21
#define GATE_TIMER_PRESCALER 250
#define SAMPLE_FREQ 125000000

#define DETECTOR_N_OF_EDGES 3
#define DETECTOR_N_OF_WIDTHS 3

#define GATE_TIME_COUNT 4

static uint32_t GATE_WRAPS[] = {125000, 62500, 12500, 6250};
static uint32_t GATE_TIMES[] = {250, 125, 25, 1};

typedef enum { DET_EDGE1_RISE, DET_EDGE2_FALL, DET_EDGE3_RISE } det_n_edge_t;

typedef enum {
    DETECTOR_MODE_FREQUENCY,
    DETECTOR_MODE_PULSE_UP,
    DETECTOR_MODE_PULSE_DOWN,
} detector_mode_t;

typedef enum { DET_LOW_WIDTH, DET_HIGH_WIDTH, DET_PERIOD_WIDTH } det_n_width_t;

typedef struct {
    uint32_t dma_channel;
    uint32_t csr_timer_stop;
    uint32_t timer_slice;
    uint32_t gate_slice;
    uint32_t gate_dma_channel;
    uint8_t gate_index;

} timer_gate_perif_t;

typedef struct {
    timer_gate_perif_t gate_perif;
    uint64_t freq;
    bool pulse_found;
    uint32_t edge_times[DETECTOR_N_OF_EDGES];
    uint32_t widths[DETECTOR_N_OF_WIDTHS];
    det_n_edge_t edge_mode;
    bool is_rec;
    bool is_rec_finished;
} sig_det_t;

void sig_det_frequecy_counter_init(timer_gate_perif_t* perif);

void sig_det_pulse_detect_init(sig_det_t* det_perif);

void sig_det_gate_timer_init(timer_gate_perif_t* perif);

void sig_det_gate_timer_deinit(timer_gate_perif_t* perif);
bool sig_det_check_ready_gate(timer_gate_perif_t* perif);

void sig_det_counters_start(timer_gate_perif_t* perif);

void sig_det_get_freq_value(sig_det_t* perif);

void sig_det_pulse_detect_callback(uint gpio, uint32_t events);
void sig_det_recproc_pulse_callback(uint gpio, uint32_t events);

#endif

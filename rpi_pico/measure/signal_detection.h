#ifndef SIGNAL_DETECTION_H
#define SIGNAL_DETECTION_H
#include <stdint.h>
#include "pico/stdlib.h"

#define FREQUECY_PIN 21
#define GATE_TIMER_PRESCALER 250  // toto vysvetlit
#define GATE_TIMER_WRAP 125000
#define SAMPLE_FREQ (125000000 / (GATE_TIMER_PRESCALER * GATE_TIMER_WRAP))

typedef enum {
    DETECTOR_MODE_FREQUENCY,
    DETECTOR_MODE_PULSE_UP,
    DETECTOR_MODE_PULSE_DOWN,
} detector_mode_t;

typedef struct {
    uint32_t dma_channel;
    uint32_t csr_timer_stop;
    uint32_t timer_slice;
    uint32_t gate_slice;
    uint32_t gate_dma_channel;

} timer_gate_perif_t;

typedef struct {
    timer_gate_perif_t gate_perif;
    uint64_t freq;
    uint64_t rec_freq;
    bool pulse_found;
} sig_det_t;

void sig_det_frequecy_counter_init(timer_gate_perif_t* perif);

void sig_det_pulse_detect_init(sig_det_t* det_perif);

void sig_det_gate_timer_init(timer_gate_perif_t* perif);

bool sig_det_check_ready_gate(timer_gate_perif_t* perif);

void sig_det_counters_start(timer_gate_perif_t* perif);

void sig_det_get_freq_value(sig_det_t* perif);

void sig_det_pulse_detect_callback(uint gpio, uint32_t events);

#endif

#ifndef GLOBAL_VAR_H
#define GLOBAL_VAR_H
#include "adc_control.h"
#include "ansi_pages.h"
#include "signal_detector.h"
#include "signal_generator.h"

#define PROCESSOR_FREQ_IN_MHZ 64

typedef enum {
    DEV_STATE_NONE,
    DEV_STATE_VOLTMETER,
    DEV_STATE_OHMMETER,
    DEV_STATE_FREQUENCY_READ,
    DEV_STATE_PULSE_READ,
    DEV_STATE_PULSE_GEN,
} dev_state_t;

typedef struct {
    dev_state_t device_state;
    unsigned char received_char;
    _Bool need_frontend_update;
    _Bool need_perif_update;
    ansi_page_type_t current_page;
    adc_vars_t* adc_vars;
    sig_detector_t* signal_detector;
    sig_generator_t* signal_generator;
} global_vars_t;

#endif

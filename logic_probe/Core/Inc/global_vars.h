#ifndef GLOBAL_VAR_H
#define GLOBAL_VAR_H
#include "adc_control.h"
#include "ansi_pages.h"
#include "signal_detector.h"
#include "signal_generator.h"

typedef enum { DEV_STATE_VOLTMETER } dev_state_t;

typedef struct {
    dev_state_t device_state;
    _Bool init;
    unsigned char received_char;
    ansi_page_type_t current_page;
    adc_vars_t* adc_vars;
    sig_detector_t* signal_detector;
    sig_generator_t* signal_generator;
} global_vars_t;

#endif

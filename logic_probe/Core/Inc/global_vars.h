#ifndef GLOBAL_VAR_H
#define GLOBAL_VAR_H
#include "adc_control.h"
#include "ansi_pages.h"
#include "extern_button.h"
#include "global_structs.h"
#include "signal_detector.h"
#include "signal_generator.h"
#include "visual_output.h"

#define PROCESSOR_FREQ_IN_MHZ 64

#define SETUP_TRIES 3
#define SETUP_WAIT 300

typedef struct {
    dev_state_t device_state;
    local_state_t local_state;
    local_substate_t local_substate;
    dev_setup_t device_setup;
    unsigned char received_char;
    _Bool need_frontend_update;
    _Bool need_perif_update;
    ansi_page_type_t current_page;
    adc_vars_t* adc_vars;
    sig_detector_t* signal_detector;
    sig_generator_t* signal_generator;
    visual_output_t* visual_output;
    button_data_t* button_data;
} global_vars_t;

#endif

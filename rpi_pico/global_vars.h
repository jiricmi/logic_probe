#ifndef GLOBAL_VAR_H
#define GLOBAL_VAR_H
#include "ansi/ansi_pages.h"
#include "global_structs.h"

#define PROCESSOR_FREQ_IN_MHZ 64

#define SETUP_TRIES 3
#define SETUP_WAIT 300

typedef struct {
    dev_state_t device_state;
    //   unsigned char received_char;

    //    _Bool need_frontend_update;
    //    _Bool need_perif_update;
    ansi_page_type_t current_page;

    // measures
    //    adc_vars_t* adc_vars;
    //    sig_detector_t* signal_detector;
    //    sig_generator_t* signal_generator;
} global_vars_t;

#endif

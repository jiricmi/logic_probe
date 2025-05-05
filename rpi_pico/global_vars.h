#ifndef GLOBAL_VAR_H
#define GLOBAL_VAR_H
#include "ansi/ansi_pages.h"
#include "global_structs.h"
#include "measure/adc_control.h"
#include "probe_usb.h"
#include "signal_detection.h"

typedef struct {
    dev_state_t device_state;
    bool need_frontend_update;
    bool need_perif_update;
    ansi_page_type_t current_page;
    usb_perif_t usb_perif;

    // measures
    adc_vars_t adc_perif;
    sig_det_t sig_det_perif;
    //    sig_generator_t* signal_generator;
} global_vars_t;

#endif

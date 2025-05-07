#ifndef GLOBAL_VAR_H
#define GLOBAL_VAR_H
#include "ansi/ansi_pages.h"
#include "global_structs.h"
#include "measure/adc_control.h"
#include "probe_usb.h"
#include "signal_detection.h"
#include "signal_generation.h"
#define LED_STATE_COUNT 3

typedef struct {
    bool booted;
    dev_state_t device_state;
    bool need_frontend_update;
    bool need_perif_update;
    ansi_page_type_t current_page;
    usb_perif_t usb_perif;

    // measures
    adc_vars_t adc_perif;
    sig_det_t sig_det_perif;
    sig_gen_t sig_gen_perif;
    bool gpio_level_state[LED_STATE_COUNT];
    //    sig_generator_t* signal_generator;
} global_vars_t;

#endif

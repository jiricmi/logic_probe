#include "control_frequency_reader.h"

#include <stdbool.h>

#include "ansi_abstraction_layer.h"
#include "global_vars.h"
#include "loop.h"
#include "signal_detector.h"

extern global_vars_t global_var;

void control_frequency_reader_page(char received_char,
                                   sig_detector_t* sig_det) {
    switch (received_char) {
        case 'q':
        case 'Q':
            ansi_clear_terminal();
            ansi_set_current_page(ANSI_PAGE_MAIN);
            dev_mode_change_mode(DEV_STATE_NONE);
            break;
        case 'm':
        case 'M':
            detector_change_mode(sig_det);
            dev_mode_request_update();
            break;
        case 't':
        case 'T':
            detector_change_sample_time(global_var.signal_detector);
            break;
        case 'd':
        case 'D':
            global_var.signal_detector->one_pulse_found = false;
            break;
    }
}

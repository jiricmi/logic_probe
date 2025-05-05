#include "control_frequency_reader.h"

#include <stdbool.h>

#include "ansi_abstraction_layer.h"
#include "global_structs.h"
#include "global_vars.h"
#include "loop.h"
#include "signal_detection.h"

extern global_vars_t global_var;

void control_frequency_reader_page(char received_char, sig_det_t* sig_det) {
    switch (received_char) {
        case 'q':
        case 'Q':
            ansi_clear_terminal();
            ansi_set_current_page(ANSI_PAGE_MAIN);
            dev_mode_change_mode(DEV_STATE_NONE);
            break;
        case 'm':
        case 'M': {
            dev_state_t state = global_var.device_state;
            if (state == DEV_STATE_DETECT_PULSE_DOWN) {
                state = DEV_STATE_FREQUENCY_READ;
            } else {
                ++state;
            }
            global_var.sig_det_perif.pulse_found = false;
            dev_mode_change_mode(state);
            dev_mode_request_frontend_change();
            break;
        }
        case 't':
        case 'T':
            sig_det->gate_perif.gate_index++;
            if (sig_det->gate_perif.gate_index >= GATE_TIME_COUNT) {
                sig_det->gate_perif.gate_index = 0;
            }
            dev_mode_request_update();
            break;
        case 'd':
        case 'D':
            global_var.sig_det_perif.pulse_found = false;
            break;
    }
}

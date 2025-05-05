#include "loop.h"
#include <stdint.h>
#include "adc_control.h"
#include "ansi_abstraction_layer.h"
#include "global_vars.h"
#include "pages/ansi_page_frequency_reader.h"
#include "pages/ansi_page_voltage_measure.h"
#include "perif/uart_control/uart_control.h"

extern global_vars_t global_var;

void dev_mode_check_update(void) {
    if (global_var.need_perif_update) {
        dev_mode_update_perif();
    }

    if (global_var.need_frontend_update) {
        global_var.need_frontend_update = false;
        ansi_clear_terminal();
        ansi_render_current_page();
    }
}

void dev_mode_perif_turn_off(adc_vars_t* adc_perif) {
    adc_stop_measure(adc_perif);
}

void dev_mode_change_mode(dev_state_t mode) {
    global_var.device_state = mode;
    dev_mode_request_update();
}

void dev_mode_request_frontend_change(void) {
    global_var.need_frontend_update = true;
}

void dev_mode_request_perif_change(void) {
    global_var.need_perif_update = true;
}

void dev_mode_request_update(void) {
    dev_mode_request_frontend_change();
    dev_mode_request_perif_change();
}

void dev_mode_update_perif(void) {
    //   sig_detector_t* sig_det = global_var.signal_detector;
    adc_vars_t* adc_perif = &global_var.adc_perif;
    // sig_generator_t* sig_gen = global_var.signal_generator;

    dev_mode_perif_turn_off(adc_perif);

    switch (global_var.device_state) {
        case DEV_STATE_NONE:
            break;
        case DEV_STATE_VOLTMETER:
        case DEV_STATE_OHMMETER:
            adc_start_measure(adc_perif);
            break;
        case DEV_STATE_FREQUENCY_READ:
            sig_det_frequecy_counter_init(&global_var.sig_det_perif.gate_perif);
            sig_det_gate_timer_init(&global_var.sig_det_perif.gate_perif);
            break;
        case DEV_STATE_PULSE_GEN:
            //   generator_setup_timers(sig_gen);
            break;
        default:
            break;
    }
    global_var.need_perif_update = false;
}

void dev_mode_run(void) {
    dev_mode_check_update();
    adc_vars_t* adc_perif = &global_var.adc_perif;
    sig_det_t* sig_det_perif = &global_var.sig_det_perif;

    uint32_t delay = 500;
    switch (global_var.device_state) {
        case DEV_STATE_NONE:
            break;
        case DEV_STATE_VOLTMETER:
            adc_get_avg_voltages(adc_perif);
            ansi_render_voltage_measures(adc_perif);
            delay = 250;
            break;
        case DEV_STATE_OHMMETER:
            adc_get_avg_voltages(adc_perif);
            ansi_render_resistance_measure(adc_perif);
            delay = 250;
            break;
        case DEV_STATE_FREQUENCY_READ:
            sig_det_counters_start(&sig_det_perif->gate_perif);
            while (sig_det_check_ready_gate(&sig_det_perif->gate_perif))
                ;
            sig_det_get_freq_value(sig_det_perif);
            ansi_generate_frequency_reader(sig_det_perif);
            break;
        case DEV_STATE_PULSE_GEN:
            //  if (global_var.signal_generator->send) {
            //      generator_send_pulse(global_var.signal_generator);
            //   }
            //   ansi_render_impulse_generator(global_var.signal_generator);
            break;
        default:
            break;
    }
    sleep_ms(delay);
}

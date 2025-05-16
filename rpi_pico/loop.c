#include "loop.h"
#include <stdint.h>
#include <stdio.h>
#include "adc_control.h"
#include "ansi_abstraction_layer.h"
#include "global_vars.h"
#include "levels.h"
#include "pages/ansi_page_frequency_reader.h"
#include "pages/ansi_page_impulse_generator.h"
#include "pages/ansi_page_levels.h"
#include "pages/ansi_page_voltage_measure.h"
#include "signal_detection.h"

extern bool led_state;
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
    gpio_set_irq_enabled_with_callback(
        FREQUECY_PIN, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, false, NULL);
    sig_det_gate_timer_deinit(&global_var.sig_det_perif.gate_perif);
    global_var.sig_det_perif.is_rec = false;
    global_var.sig_det_perif.is_rec_finished = true;
    sig_gen_deinit_perif(&global_var.sig_gen_perif);
    sig_gen_stop_pwm(&global_var.sig_gen_perif);
    gpio_deinit(GEN_PIN);
    gpio_deinit(PWM_PIN);

    deinit_level_gpio();
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
    adc_vars_t* adc_perif = &global_var.adc_perif;
    sig_det_t* sig_det_perif = &global_var.sig_det_perif;
    sig_gen_t* sig_gen_perif = &global_var.sig_gen_perif;

    dev_mode_perif_turn_off(adc_perif);

    switch (global_var.device_state) {
        case DEV_STATE_NONE:
            break;
        case DEV_STATE_VOLTMETER:
        case DEV_STATE_OHMMETER:
            adc_start_measure(adc_perif);
            break;
        case DEV_STATE_FREQUENCY_READ:
            sig_det_frequecy_counter_init(&sig_det_perif->gate_perif);
            sig_det_gate_timer_init(&sig_det_perif->gate_perif);
            break;
        case DEV_STATE_DETECT_PULSE_UP:
        case DEV_STATE_DETECT_PULSE_DOWN:
            sig_det_pulse_detect_init(sig_det_perif);
            break;
        case DEV_STATE_PULSE_GEN:
            gpio_init(GEN_PIN);
            gpio_set_dir(GEN_PIN, GPIO_OUT);
            if (sig_gen_perif->mode == SIG_GEN_MODE_PULSE_UP) {
                gpio_put(GEN_PIN, false);
            } else {
                gpio_put(GEN_PIN, true);
            }
            gpio_init(PWM_PIN);
            gpio_set_dir(PWM_PIN, GPIO_OUT);
            break;
        case DEV_STATE_LEVEL:
            init_level_gpio();
            break;
        default:
            break;
    }
    global_var.need_perif_update = false;
}

void dev_mode_run(void) {
    gpio_put(PICO_DEFAULT_LED_PIN, led_state = !led_state);
    dev_mode_check_update();
    adc_vars_t* adc_perif = &global_var.adc_perif;
    sig_det_t* sig_det_perif = &global_var.sig_det_perif;
    sig_gen_t* sig_gen_perif = &global_var.sig_gen_perif;

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
            delay = 500;
            break;
        case DEV_STATE_FREQUENCY_READ:
            if (!global_var.sig_det_perif.is_rec) {
                sig_det_counters_start(&sig_det_perif->gate_perif);
                while (sig_det_check_ready_gate(&sig_det_perif->gate_perif))
                    ;
                sig_det_get_freq_value(sig_det_perif);
                sig_det_perif->is_rec = true;
            } else {
                if (global_var.sig_det_perif.is_rec_finished) {
                    sig_det_perif->is_rec_finished = 0;
                    gpio_set_irq_enabled_with_callback(
                        FREQUECY_PIN, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE,
                        true, &sig_det_recproc_pulse_callback);
                }
                delay = 250;
            }
        case DEV_STATE_DETECT_PULSE_UP:
        case DEV_STATE_DETECT_PULSE_DOWN:
            ansi_generate_frequency_reader(sig_det_perif);
            break;
        case DEV_STATE_PULSE_GEN:
            if (sig_gen_perif->send) {
                sig_gen_send_perif(sig_gen_perif);

                sig_gen_perif->send = 0;
            }
            ansi_render_impulse_generator(sig_gen_perif);
            break;
        case DEV_STATE_LEVEL:
            ansi_render_levels_page();
            break;
        default:
            break;
    }
    sleep_ms(delay);
}

#include "loop.h"
#include "adc_control.h"
#include "advanced/neopixel.h"
#include "ansi_page_frequency_reader.h"
#include "ansi_page_impulse_generator.h"
#include "ansi_page_voltage_measure.h"
#include "ansi_pages_neopixel_measure.h"
#include "global_vars.h"
#include "gpio_outputs.h"
#include "signal_detector.h"
#include "tim_setup.h"

#include <stdbool.h>

extern global_vars_t global_var;

dev_setup_t dev_mode_get_dev_setup(void) {
    for (uint8_t i = 0; i < SETUP_TRIES; ++i) {
        if (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10)) {
            HAL_Delay(SETUP_WAIT);
            continue;
        } else {
            return DEV_SETUP_UART;
        }
    }
    return DEV_SETUP_LOCAL;
}

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

void dev_mode_run_with_uart(void) {
    dev_mode_check_update();

    uint32_t delay = 500;
    switch (global_var.device_state) {
        case DEV_STATE_NONE:
            break;
        case DEV_STATE_VOLTMETER:
            adc_get_avg_voltages(global_var.adc_vars);
            ansi_render_voltage_measures(global_var.adc_vars);
            delay = 300;
            break;
        case DEV_STATE_OHMMETER:
            ansi_render_resistance_measure(global_var.adc_vars);
            break;
        case DEV_STATE_FREQUENCY_READ:
            ansi_generate_frequency_reader(global_var.signal_detector);
            break;
        case DEV_STATE_PULSE_GEN:
            ansi_render_impulse_generator(global_var.signal_generator);
            break;
        case DEV_STATE_ADV_NEOPIXEL_READ:
            delay = 200;
            ansi_render_neopixel_measure_vals(global_var.adv_neopixel_measure);
            break;
        case DEV_STATE_ADV_NEOPIXEL_WRITE:
            break;
        case DEV_STATE_ADV_SHIFT_REGISTER:
            if (global_var.adv_shift_register->ready_to_send) {
                shift_register_send_signal(global_var.adv_shift_register);
            }
            break;
        default:
            break;
    }
    HAL_Delay(delay);
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

void dev_mode_perif_turn_off(sig_detector_t* sig_det, adc_vars_t* adc_vars) {
    HAL_ADC_Stop_DMA(adc_vars->hadc);
    HAL_TIM_Base_Stop_IT(sig_det->master_tim);
    HAL_TIM_Base_Stop(sig_det->slave_tim);
    HAL_TIM_IC_Stop_IT(sig_det->slave_tim, TIM_CHANNEL_1);
    HAL_TIM_IC_Stop_IT(sig_det->slave_tim, TIM_CHANNEL_2);
    HAL_TIM_IC_Stop_DMA(sig_det->slave_tim, TIM_CHANNEL_1);
    HAL_TIM_IC_Stop_DMA(sig_det->slave_tim, TIM_CHANNEL_2);

    __HAL_TIM_SET_COUNTER(sig_det->master_tim, 0);
    __HAL_TIM_SET_COUNTER(sig_det->slave_tim, 0);
    gpio_init_timer();
}

void dev_mode_update_perif(void) {
    sig_detector_t* sig_det = global_var.signal_detector;
    adc_vars_t* adc_vars = global_var.adc_vars;
    sig_generator_t* sig_gen = global_var.signal_generator;
    neopixel_measure_t* neopixel_measure = global_var.adv_neopixel_measure;

    dev_mode_perif_turn_off(sig_det, adc_vars);

    switch (global_var.device_state) {
        case DEV_STATE_NONE:
            break;
        case DEV_STATE_VOLTMETER:
        case DEV_STATE_OHMMETER:
            adc_remove_unapplied_channels(global_var.adc_vars);
            HAL_ADC_Start_DMA(
                global_var.adc_vars->hadc,
                global_var.adc_vars->voltage_measures,
                global_var.adc_vars->n_active_channels * CHANNEL_NUM_SAMPLES);
            break;
        case DEV_STATE_FREQUENCY_READ:
            detector_setup_timers(sig_det, false);
            break;
        case DEV_STATE_PULSE_GEN:
            generator_setup_timers(sig_gen);
            break;
        case DEV_STATE_ADV_NEOPIXEL_READ:
            adv_neopixel_read_init_timers(neopixel_measure);
            neopixel_read_start(global_var.adv_neopixel_measure);
            break;
        case DEV_STATE_ADV_NEOPIXEL_WRITE:
            break;
        case DEV_STATE_ADV_SHIFT_REGISTER:
            gpio_init_push_pull();
            break;
        default:
            break;
    }
    global_var.need_perif_update = false;
}

void local_mode_update_perif(void) {
    sig_detector_t* sig_det = global_var.signal_detector;
    adc_vars_t* adc_vars = global_var.adc_vars;

    dev_mode_perif_turn_off(sig_det, adc_vars);

    switch (global_var.local_state) {
        case LOCAL_STATE_LOGIC_PROBE:
        case LOCAL_STATE_VOLTMETER_PROBE:
            global_var.device_state = DEV_STATE_VOLTMETER;
            for (uint8_t i = 1; i < ADC_NUM_CHANNELS; ++i) {
                adc_vars->channel_state_unapplied[i] = true;
            }
            adc_apply_channels(global_var.adc_vars);
            adc_setup_channel_struct(global_var.adc_vars);
            HAL_ADC_Start_DMA(
                global_var.adc_vars->hadc,
                global_var.adc_vars->voltage_measures,
                global_var.adc_vars->n_active_channels * CHANNEL_NUM_SAMPLES);
            break;
        case LOCAL_STATE_OUTPUT:
            gpio_init_push_pull();
            break;
        case LOCAL_STATE_PULSEUP:
            global_var.device_state = DEV_STATE_FREQUENCY_READ;
            sig_det->mode = DETECTOR_MODE_PULSE_UP;
            detector_setup_timers(sig_det, false);
            break;
        case LOCAL_STATE_PULSEDOWN:
            global_var.device_state = DEV_STATE_FREQUENCY_READ;
            sig_det->mode = DETECTOR_MODE_PULSE_DOWN;
            detector_setup_timers(sig_det, false);
            break;
        default:
            break;
    }
    global_var.need_perif_update = false;
}

void dev_mode_run(void) {
    adc_vars_t* adc_vars = global_var.adc_vars;
    sig_detector_t* sig_det = global_var.signal_detector;
    uint32_t delay = 500;
    if (global_var.button_data->long_press || global_var.need_perif_update) {
        if (!global_var.need_perif_update) {
            extern_button_handle_long(global_var.button_data);
        }
        local_mode_update_perif();
    }

    if (global_var.button_data->short_press) {
        extern_button_handle_short(global_var.button_data);
    }

    if (global_var.button_data->double_press) {
        extern_button_handle_double(global_var.button_data);
    }

    switch (global_var.local_state) {
        case LOCAL_STATE_LOGIC_PROBE: {
            adc_get_avg_voltages(global_var.adc_vars);
            probe_state_t probe_state =
                adc_local_logic_probe(adc_vars, global_var.local_substate);
            neopixel_show_probe_state(global_var.visual_output, probe_state);
            break;
        }
        case LOCAL_STATE_VOLTMETER_PROBE: {
            uint32_t floating_avg_measures[ADC_NUM_CHANNELS];
            adc_get_avg_voltages(global_var.adc_vars);
            adc_calculate_floating_voltage_avg(floating_avg_measures, adc_vars);

            uint8_t index =
                (global_var.local_substate == LOCAL_SUBSTATE_CHANNEL_1) ? 1 : 2;
            neopixel_show_voltmeter_state(global_var.visual_output,
                                          global_var.local_substate,
                                          floating_avg_measures[index]);
            break;
        }
        case LOCAL_STATE_OUTPUT: {
            neopixel_show_output_state(global_var.visual_output,
                                       global_var.local_substate);
            break;
        }
        case LOCAL_STATE_PULSEUP:
        case LOCAL_STATE_PULSEDOWN:
            if (sig_det->one_pulse_found) {
                sig_det->one_pulse_found = false;
                neopixel_show_pulse_detection(global_var.visual_output,
                                              sig_det);
            } else {
                neopixel_send_color(global_var.visual_output, NEOPIXEL_NONE);
            }
        default:
            break;
    }
    HAL_Delay(delay);
}

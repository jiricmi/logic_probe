#include "loop.h"
#include "adc_control.h"
#include "ansi_abstraction_layer.h"
#include "ansi_page_frequency_reader.h"
#include "ansi_page_voltage_measure.h"
#include "global_vars.h"
#include "signal_detector.h"

#include <stdbool.h>

extern global_vars_t global_var;

void dev_mode_run_with_uart(void) {
    if (global_var.need_perif_update) {
        dev_mode_update_perif();
    }

    if (global_var.need_frontend_update) {
        global_var.need_frontend_update = false;
        ansi_clear_terminal();
        ansi_render_current_page();
    }

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
    __HAL_TIM_SET_COUNTER(sig_det->master_tim, 0);
    __HAL_TIM_SET_COUNTER(sig_det->slave_tim, 0);
}

void dev_mode_update_perif(void) {
    sig_detector_t* sig_det = global_var.signal_detector;
    adc_vars_t* adc_vars = global_var.adc_vars;

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
            HAL_TIM_Base_Start_IT(global_var.signal_detector->master_tim);
            break;

        default:
            break;
    }
    global_var.need_perif_update = false;
}

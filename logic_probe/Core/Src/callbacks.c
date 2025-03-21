#include "callbacks.h"
#include <stdbool.h>
#include "ansi_abstraction_layer.h"
#include "ansi_pages.h"
#include "global_vars.h"
#include "main.h"
#include "stm32g0xx_hal_tim.h"
#include "tim_setup.h"
#include "uart_control.h"
#include "visual_output.h"

extern global_vars_t global_var;

extern UART_HandleTypeDef UART;

extern TIM_HandleTypeDef htim14;
extern TIM_HandleTypeDef htim16;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {
    if (huart->Instance == UART_INST) {
        if (global_var.received_char == 'r' ||
            global_var.received_char == 'R') {  // reload
            ansi_clear_terminal();
            ansi_render_current_page();
        } else {
            get_current_control();
        }

        HAL_UART_Receive_IT(&UART, &global_var.received_char, 1);
    }
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
    if (htim->Instance == global_var.signal_detector->master_tim->Instance) {
        sig_detector_t* sig_det = global_var.signal_detector;

        if (sig_det->slave_tim_mode == SLAVE_MODE_FREQUENCY) {
            HAL_TIM_Base_Stop(sig_det->slave_tim);
            uint32_t n_pulses = __HAL_TIM_GET_COUNTER(sig_det->slave_tim);

            sig_det->frequency =
                detector_get_gated_value(n_pulses, sig_det->sample_time_index);

            HAL_TIM_Base_Stop_IT(sig_det->master_tim);
            detector_slave_init_pulse_width(sig_det);
            HAL_TIM_IC_Start_IT(sig_det->slave_tim, TIM_CHANNEL_1);
            HAL_TIM_IC_Start_IT(sig_det->slave_tim, TIM_CHANNEL_2);

        } else if (sig_det->slave_tim_mode == SLAVE_MODE_WIDTH) {
            HAL_TIM_IC_Stop_IT(sig_det->slave_tim, TIM_CHANNEL_1);
            HAL_TIM_IC_Stop_IT(sig_det->slave_tim, TIM_CHANNEL_2);
            HAL_TIM_Base_Stop_IT(sig_det->master_tim);
            detector_slave_init_frequency(sig_det);
        }

        __HAL_TIM_SET_COUNTER(sig_det->master_tim, 0);
        HAL_TIM_Base_Start_IT(sig_det->master_tim);
    }
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim) {
    sig_detector_t* sig_det = global_var.signal_detector;
    sig_generator_t* sig_gen = global_var.signal_generator;
    dev_state_t state = global_var.device_state;

    if (htim->Instance == sig_det->slave_tim->Instance &&
        state == DEV_STATE_FREQUENCY_READ) {
        if (sig_det->mode == DETECTOR_MODE_FREQUENCY) {
            detector_parse_catched_signal(sig_det);
        } else {
            // TODO: UDELAT MERENI JAK DLOUHY JE PULS
            detector_parse_pulse_catcher(sig_det);
        }
    } else if (htim->Instance == sig_gen->htim->Instance &&
               state == DEV_STATE_PULSE_GEN) {
        HAL_TIM_PWM_Stop(global_var.signal_generator->htim, TIM_CHANNEL_1);
    }
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef* htim) {
    if (htim->Instance == global_var.visual_output->neopixel_tim->Instance) {
        HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_3);
    }
}
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == global_var.button_data->pin) {
        button_data_t* button_data = global_var.button_data;

        if (!button_data->is_pressed) {
            button_data->rise_edge_time = HAL_GetTick();
            button_data->is_pressed = true;
        }
    }
}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == global_var.button_data->pin) {
        button_data_t* button_data = global_var.button_data;

        if (button_data->is_pressed) {
            button_data->fall_edge_time = HAL_GetTick();
            extern_button_check_press(button_data);
            button_data->is_pressed = false;
        }
    }
}

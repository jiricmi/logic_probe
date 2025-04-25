#include "callbacks.h"
#include <stdbool.h>
#include "ansi_abstraction_layer.h"
#include "ansi_pages.h"
#include "global_vars.h"
#include "loop.h"
#include "main.h"
#include "stm32g0xx_hal_i2c.h"
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
    if (htim->Instance == global_var.signal_detector->master_tim->Instance &&
        global_var.device_state == DEV_STATE_FREQUENCY_READ) {
        sig_detector_t* sig_det = global_var.signal_detector;

        if (sig_det->slave_tim_mode == SLAVE_MODE_FREQUENCY) {
            HAL_TIM_Base_Stop(sig_det->slave_tim);
            uint32_t n_pulses = __HAL_TIM_GET_COUNTER(sig_det->slave_tim);

            sig_det->frequency =
                detector_get_gated_value(n_pulses, sig_det->gate_time_index);

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
    } else if (htim->Instance == global_var.adc_vars->timer->Instance &&
               (global_var.device_state == DEV_STATE_VOLTMETER ||
                global_var.device_state == DEV_STATE_OHMMETER)) {
        adc_vars_t* adc_perif = global_var.adc_vars;
        HAL_TIM_Base_Stop_IT(adc_perif->timer);

        HAL_ADC_Start(adc_perif->hadc);
        for (uint8_t i = 0; i < adc_perif->n_active_channels; ++i) {
            HAL_ADC_PollForConversion(adc_perif->hadc, 100);
            adc_perif->voltage_measures[adc_perif->measures_index++] =
                HAL_ADC_GetValue(adc_perif->hadc);

            if (adc_perif->measures_index >=
                adc_perif->n_active_channels * CHANNEL_NUM_SAMPLES) {
                adc_perif->measures_index = 0;
            }
        }
        HAL_ADC_Stop(adc_perif->hadc);
        __HAL_TIM_SET_COUNTER(adc_perif->timer, 0);
        HAL_TIM_Base_Start_IT(adc_perif->timer);
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
            detector_parse_pulse_catcher(sig_det);
        }
    } else if (htim->Instance == sig_gen->htim->Instance &&
               state == DEV_STATE_PULSE_GEN) {
        HAL_TIM_PWM_Stop(global_var.signal_generator->htim, TIM_CHANNEL_1);
    }
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef* htim) {
    if (htim->Instance == global_var.visual_output->neopixel_tim->Instance) {
        global_var.adv_neopixel_measure->count++;
        if (global_var.adv_neopixel_measure->rgb_send[3] ==
            global_var.adv_neopixel_measure->count) {
            HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_3);
            global_var.adv_neopixel_measure->count = 0;
        }
    }
}
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == global_var.button_data->pin) {
        button_data_t* button_data = global_var.button_data;

        if (!button_data->is_pressed) {
            button_data->rise_edge_time = HAL_GetTick();
            button_data->is_pressed = true;
        }
    }
}

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == global_var.button_data->pin) {
        button_data_t* button_data = global_var.button_data;

        if (button_data->is_pressed) {
            button_data->fall_edge_time = HAL_GetTick();
            extern_button_check_press(button_data);
            button_data->is_pressed = false;
        }
    }
}

extern void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef* hi2c) {
    HAL_I2C_EnableListen_IT(hi2c);
}
extern void HAL_I2C_AddrCallback(I2C_HandleTypeDef* hi2c,
                                 uint8_t TransferDirection,
                                 uint16_t AddrMatchCode) {
    if (TransferDirection == I2C_DIRECTION_TRANSMIT) {
        HAL_I2C_Slave_Sequential_Receive_IT(
            hi2c, global_var.i2c_perif->slave_received_data,
            global_var.i2c_perif->bytes_to_catch, I2C_FIRST_AND_LAST_FRAME);
    } else {
        Error_Handler();
    }
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef* hi2c) {
    HAL_I2C_EnableListen_IT(hi2c);
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef* hi2c) {
    dev_mode_request_frontend_change();
}

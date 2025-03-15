#include "callbacks.h"
#include <stdbool.h>
#include "ansi_abstraction_layer.h"
#include "ansi_pages.h"
#include "global_vars.h"
#include "main.h"
#include "stm32g0xx_hal_tim.h"
#include "uart_control.h"

extern global_vars_t global_var;

extern UART_HandleTypeDef huart2;

extern TIM_HandleTypeDef htim14;
extern TIM_HandleTypeDef htim16;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {
    if (huart->Instance == USART2) {
        if (global_var.received_char == 'r' ||
            global_var.received_char == 'R') {  // reload
            ansi_clear_terminal();
            ansi_render_current_page();
        } else {
            get_current_control();
        }

        HAL_UART_Receive_IT(&huart2, &global_var.received_char, 1);
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

    } else if (htim->Instance == TIM16) {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7);
        if (global_var.signal_generator->con) {
        } else if (!global_var.signal_generator->start) {
            global_var.signal_generator->start = true;
        } else {
            HAL_TIM_Base_Stop_IT(&htim16);
            global_var.signal_generator->start = false;
        }
    }
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim) {
    sig_detector_t* sig_det = global_var.signal_detector;
    if (htim->Instance == sig_det->slave_tim->Instance) {
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
            if (!sig_det->edge_catch[DET_EDGE1_RISE]) {
                sig_det->edge_times[DET_EDGE1_RISE] =
                    HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
                sig_det->edge_catch[DET_EDGE1_RISE] = true;
            } else if (!sig_det->edge_catch[DET_EDGE3_RISE]) {
                sig_det->edge_times[DET_EDGE3_RISE] =
                    HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
                sig_det->edge_catch[DET_EDGE3_RISE] = true;

                detector_compute_freq_measures(sig_det);

                HAL_TIM_IC_Stop_IT(sig_det->slave_tim, TIM_CHANNEL_1);
                HAL_TIM_IC_Stop_IT(sig_det->slave_tim, TIM_CHANNEL_2);
            }

        } else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2) {
            if (sig_det->edge_catch[DET_EDGE1_RISE] &&
                !sig_det->edge_catch[DET_EDGE2_FALL]) {
                sig_det->edge_times[DET_EDGE2_FALL] =
                    HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
                sig_det->edge_catch[DET_EDGE2_FALL] = true;
            }
        }
    }
}

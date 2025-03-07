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

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
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
    if (htim->Instance == TIM3) {
        global_var.signal_detector->frequency =
            (uint32_t)((float)global_var.signal_detector->pulse_count / 2) /
            ((float)global_var.signal_detector
                 ->sample_times[global_var.signal_detector->sample_time_index] /
             1000);

        uint32_t rise_pulse_ticks;
        if (global_var.signal_detector->high_end >
            global_var.signal_detector->high_start) {
            rise_pulse_ticks = global_var.signal_detector->high_end -
                               global_var.signal_detector->high_start;
        } else if (global_var.signal_detector->high_end <
                   global_var.signal_detector->high_start) {
            rise_pulse_ticks =
                (0xFFFFFFFF - global_var.signal_detector->high_start) +
                global_var.signal_detector->high_end;
        } else {
            rise_pulse_ticks = 0;
        }
        global_var.signal_detector->high_width =
            (rise_pulse_ticks * 15.625) / 1000;

        if (global_var.signal_detector->low_end >
            global_var.signal_detector->low_start) {
            rise_pulse_ticks = global_var.signal_detector->low_end -
                               global_var.signal_detector->low_start;
        } else if (global_var.signal_detector->low_end <
                   global_var.signal_detector->low_start) {
            rise_pulse_ticks =
                (0xFFFFFFFF - global_var.signal_detector->low_start) +
                global_var.signal_detector->low_end;
        } else {
            rise_pulse_ticks = 0;
        }
        global_var.signal_detector->low_width =
            (rise_pulse_ticks * 15.625) / 1000;

        HAL_TIM_Base_Stop_IT(&htim3);
        HAL_TIM_IC_Stop_IT(&htim2, TIM_CHANNEL_1);
        HAL_TIM_IC_Stop_IT(&htim2, TIM_CHANNEL_2);

        global_var.signal_detector->pulse_count = 0;
        HAL_TIM_Base_Start_IT(&htim14);

    } else if (htim->Instance == TIM14) {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
        HAL_TIM_Base_Stop_IT(&htim14);

        if (global_var.signal_detector->high_calculated) {
            global_var.signal_detector->high_catched = false;
            global_var.signal_detector->high_calculated = false;
        }

        if (global_var.signal_detector->low_calculated) {
            global_var.signal_detector->low_catched = false;
            global_var.signal_detector->low_calculated = false;
        }
        HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
        HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2);
        global_var.signal_detector->pulse_count = 0;
        HAL_TIM_Base_Start_IT(&htim3);
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
    if (htim->Instance == TIM2) {
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
            if (!global_var.signal_detector->high_calculated &&
                !global_var.signal_detector->high_catched) {
                global_var.signal_detector->high_start =
                    HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_1);
                global_var.signal_detector->high_catched = true;
            }
            if (!global_var.signal_detector->low_calculated &&
                global_var.signal_detector->low_catched) {
                global_var.signal_detector->low_end =
                    HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_1);
                global_var.signal_detector->low_calculated = true;
            }
            if (global_var.signal_detector->mode == 0) {
                global_var.signal_detector->p = true;
            }
        }
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2) {
            if (!global_var.signal_detector->high_calculated &&
                global_var.signal_detector->high_catched) {
                global_var.signal_detector->high_end =
                    HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_2);
                global_var.signal_detector->high_calculated = true;
            }
            if (!global_var.signal_detector->low_calculated &&
                !global_var.signal_detector->low_catched) {
                global_var.signal_detector->low_start =
                    HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_2);
                global_var.signal_detector->low_catched = true;
            }
            if (global_var.signal_detector->mode == 1) {
                global_var.signal_detector->p = true;
            }
        }

        ++global_var.signal_detector->pulse_count;
    }
}

#include "signal_detector.h"
#include <stdbool.h>
#include <string.h>
#include "global_vars.h"
#include "main.h"
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim14;
extern TIM_HandleTypeDef htim16;

void init_detector(sig_detector_t* detector,
                   TIM_HandleTypeDef* slave_tim,
                   TIM_HandleTypeDef* master_tim) {
    memset(detector, 0, sizeof(*detector));
    detector->slave_tim = slave_tim;
    detector->master_tim = master_tim;
    detector->slave_tim_mode = SLAVE_MODE_FREQUENCY;
    detector->mode = DETECTOR_MODE_FREQUENCY;
    detector->sample_time_index = DETECTOR_BASE_SAMPLE_TIME;
}

void detector_change_sample_time(sig_detector_t* detector) {
    if (detector->sample_time_index == 0) {
        detector->sample_time_index = DETECTOR_SAMPLE_TIMES_COUNT - 1;
    } else {
        --detector->sample_time_index;
    }
    __HAL_TIM_SET_AUTORELOAD(&htim3,
                             SAMPLE_TIMES[detector->sample_time_index] - 1);
}

void detector_change_mode(sig_detector_t* detector) {
    if (detector->mode != DETECTOR_MODE_FREQUENCY) {
        detector->mode = detector->mode + 1;
    } else {
        detector->mode = DETECTOR_MODE_PULSE_UP;
    }
    detector->one_pulse_found = false;
}

uint32_t detector_get_gated_value(uint32_t n_pulses, uint8_t index) {
    uint32_t frequency;

    if (SAMPLE_TIMES[index] < 1000) {
        frequency = n_pulses * SAMPLE_TIMES_1000[index];
    } else {
        frequency = n_pulses / SAMPLE_TIMES_1000[index];
    }
    return frequency;
}

void detector_compute_freq_measures(sig_detector_t* detector) {
    uint32_t* edge_times = detector->edge_times;

    uint32_t high_delta =
        edge_times[DET_EDGE2_FALL] - edge_times[DET_EDGE1_RISE];

    uint32_t low_delta =
        edge_times[DET_EDGE3_RISE] - edge_times[DET_EDGE2_FALL];

    detector->pwm_duty = (high_delta * 1000) / (edge_times[DET_EDGE3_RISE] -
                                                edge_times[DET_EDGE1_RISE]);

    detector->widths[DET_LOW_WIDTH] = (low_delta * 10) / PROCESSOR_FREQ_IN_MHZ;
    detector->widths[DET_HIGH_WIDTH] =
        (high_delta * 10) / PROCESSOR_FREQ_IN_MHZ;
}

void detector_slave_init_pulse_width(sig_detector_t* detector) {
    TIM_HandleTypeDef* slave_tim = detector->slave_tim;

    HAL_StatusTypeDef status = HAL_TIM_Base_DeInit(slave_tim);
    if (status != HAL_OK) {
        // Chyba při deinicializaci
    }

    detector->slave_tim_mode = SLAVE_MODE_WIDTH;

    for (uint8_t i = 0; i < DETECTOR_N_OF_EDGES; ++i) {
        detector->edge_catch[i] = false;
    }

    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_IC_InitTypeDef sConfigIC = {0};

    /* USER CODE BEGIN TIM2_Init 1 */

    /* USER CODE END TIM2_Init 1 */
    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 0;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 4294967295;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim2) != HAL_OK) {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_TIM_IC_Init(&htim2) != HAL_OK) {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) !=
        HAL_OK) {
        Error_Handler();
    }
    sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter = 0;
    if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK) {
        Error_Handler();
    }
    sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
    sConfigIC.ICSelection = TIM_ICSELECTION_INDIRECTTI;
    if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_2) != HAL_OK) {
        Error_Handler();
    }
}

void detector_slave_init_frequency(sig_detector_t* detector) {
    TIM_HandleTypeDef* slave_tim = detector->slave_tim;
    HAL_StatusTypeDef status = HAL_TIM_Base_DeInit(detector->slave_tim);
    if (status != HAL_OK) {
        // Chyba při deinicializaci
    }
    detector->slave_tim_mode = SLAVE_MODE_FREQUENCY;

    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_SlaveConfigTypeDef sSlaveConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    /* USER CODE BEGIN TIM2_Init 1 */

    /* USER CODE END TIM2_Init 1 */
    slave_tim->Instance = TIM2;
    slave_tim->Init.Prescaler = 0;
    slave_tim->Init.CounterMode = TIM_COUNTERMODE_UP;
    slave_tim->Init.Period = 4294967295;
    slave_tim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    slave_tim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(slave_tim) != HAL_OK) {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_ETRMODE2;
    sClockSourceConfig.ClockPolarity = TIM_CLOCKPOLARITY_NONINVERTED;
    sClockSourceConfig.ClockPrescaler = TIM_CLOCKPRESCALER_DIV1;
    sClockSourceConfig.ClockFilter = 0;
    if (HAL_TIM_ConfigClockSource(slave_tim, &sClockSourceConfig) != HAL_OK) {
        Error_Handler();
    }
    sSlaveConfig.SlaveMode = TIM_SLAVEMODE_TRIGGER;
    sSlaveConfig.InputTrigger = TIM_TS_ITR2;
    if (HAL_TIM_SlaveConfigSynchro(slave_tim, &sSlaveConfig) != HAL_OK) {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(slave_tim, &sMasterConfig) !=
        HAL_OK) {
        Error_Handler();
    }
}

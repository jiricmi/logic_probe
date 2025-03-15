#include "tim_setup.h"

#include <stdbool.h>

#include "global_vars.h"

extern global_vars_t global_var;

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
    slave_tim->Instance = TIM2;
    slave_tim->Init.Prescaler = 0;
    slave_tim->Init.CounterMode = TIM_COUNTERMODE_UP;
    slave_tim->Init.Period = 4294967295;
    slave_tim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    slave_tim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(slave_tim) != HAL_OK) {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(slave_tim, &sClockSourceConfig) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_TIM_IC_Init(slave_tim) != HAL_OK) {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(slave_tim, &sMasterConfig) !=
        HAL_OK) {
        Error_Handler();
    }
    sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter = 0;
    if (HAL_TIM_IC_ConfigChannel(slave_tim, &sConfigIC, TIM_CHANNEL_1) !=
        HAL_OK) {
        Error_Handler();
    }
    sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
    sConfigIC.ICSelection = TIM_ICSELECTION_INDIRECTTI;
    if (HAL_TIM_IC_ConfigChannel(slave_tim, &sConfigIC, TIM_CHANNEL_2) !=
        HAL_OK) {
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

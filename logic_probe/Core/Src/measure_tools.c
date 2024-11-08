#include "measure_tools.h"
#include "main.h"

#define V_REGISTER_SIZE 4095.0

extern ADC_HandleTypeDef hadc1;

uint32_t adc_measure_Vref(void) {
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = ADC_CHANNEL_VREFINT;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        Error_Handler();
    }
    HAL_ADC_Start(&hadc1);
    uint32_t raw_v_val;
    if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK) {
        raw_v_val = HAL_ADC_GetValue(&hadc1);
        HAL_ADC_Stop(&hadc1);
        return __HAL_ADC_CALC_VREFANALOG_VOLTAGE(raw_v_val, ADC_RESOLUTION_12B);
    } else {
        HAL_ADC_Stop(&hadc1);
        return 0;
    }
}

uint32_t adc_measure_PA0(uint32_t V_ref) {
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = ADC_CHANNEL_0;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        Error_Handler();
    }

    HAL_ADC_Start(&hadc1);

    uint32_t raw_v_val;
    if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK) {
        raw_v_val = HAL_ADC_GetValue(&hadc1);
        HAL_ADC_Stop(&hadc1);
        return __HAL_ADC_CALC_DATA_TO_VOLTAGE(V_ref, raw_v_val,
                                              ADC_RESOLUTION_12B);
    } else {
        HAL_ADC_Stop(&hadc1);
        return 0;
    }
}

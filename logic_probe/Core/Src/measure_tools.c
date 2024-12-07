#include "measure_tools.h"
#include "adc_control.h"

#define V_REGISTER_SIZE 4095.0

extern ADC_HandleTypeDef hadc1;
extern uint32_t v_ref;

uint32_t adc_measure_v_ref(void) {
    // HAL_ADC_Start(&hadc1);
    uint32_t raw_voltage_value = adc_raw_measure(&hadc1);
    return __HAL_ADC_CALC_VREFANALOG_VOLTAGE(raw_voltage_value,
                                             ADC_RESOLUTION_12B);
}

uint32_t adc_measure_channel(ADC_HandleTypeDef* hadc) {
    uint32_t raw_voltage_value = adc_raw_measure(hadc);
    return __HAL_ADC_CALC_DATA_TO_VOLTAGE(v_ref, raw_voltage_value,
                                          ADC_RESOLUTION_12B);
}

void adc_measure_channels(uint32_t* v_measures,
                          ADC_HandleTypeDef* hadc,
                          const adc_channels* adc_ch) {
    for (size_t i = 0; i < 4; ++i) {
        if (adc_ch->channel[i]) {
            HAL_Delay(1000);
            v_measures[i] = adc_measure_channel(hadc);
        }
    }
}

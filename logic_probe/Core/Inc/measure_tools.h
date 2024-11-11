#ifndef MEASURE_TOOLS_H
#define MEASURE_TOOLS_H
#include <stdint.h>
#include "adc_control.h"
#include "main.h"

uint32_t adc_measure_v_ref(void);
uint32_t adc_measure_channel(ADC_HandleTypeDef* hadc);
void adc_measure_channels(uint32_t* v_measures,
                          ADC_HandleTypeDef* hadc,
                          const adc_channels* adc_ch);
#endif

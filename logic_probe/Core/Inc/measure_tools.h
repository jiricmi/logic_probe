#ifndef MEASURE_TOOLS_H
#define MEASURE_TOOLS_H
#include <stdint.h>

uint32_t adc_measure_Vref(void);
uint32_t adc_measure_PA0(uint32_t V_ref);
#endif

#ifndef ADC_CONTROL_H
#define ADC_CONTROL_H
#include "my_bool.h"

typedef struct {
    bool channel_1;
    bool channel_2;
    bool channel_3;
    bool channel_4;
} adc_channels;

adc_channels* create_adc_channels(void);

#endif  // !ADC_CONTROL_H

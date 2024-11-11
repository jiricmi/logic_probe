#ifndef ADC_CONTROL_H
#define ADC_CONTROL_H
#include <stdio.h>
#include "my_bool.h"

#define NUM_CHANNELS 4

typedef struct {
    bool channel[NUM_CHANNELS];
    bool applied;
    unsigned int pin[NUM_CHANNELS];
    unsigned int count_active;
} adc_channels;

adc_channels* create_adc_channels(void);

void flip_adc_channel(adc_channels* adc_ch, size_t channel);

size_t count_active_channels(adc_channels* adc_ch);

#endif  // !ADC_CONTROL_H

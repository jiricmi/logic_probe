#ifndef ADC_CONTROL_H
#define ADC_CONTROL_H
#include <stdio.h>
#include "main.h"
#include "my_bool.h"

#define NUM_CHANNELS 4

typedef struct {
    bool channel[NUM_CHANNELS];
    bool applied;
    unsigned int pin[NUM_CHANNELS];
    unsigned int count_active;
    ADC_HandleTypeDef* hadc;
} adc_channels;

adc_channels* create_adc_channels(ADC_HandleTypeDef* hadc);

void flip_adc_channel(adc_channels* adc_ch, size_t channel);

size_t count_active_channels(adc_channels* adc_ch);

void set_adc_rank(ADC_ChannelConfTypeDef* sConfig, size_t rank);

void set_adc_channel(ADC_ChannelConfTypeDef* sConfig, size_t channel);

void adc_init_hal_conversion(ADC_HandleTypeDef* hadc,
                             const uint32_t n_conversion);

void setup_adc_channels(ADC_HandleTypeDef* hadc,
                        adc_channels* adc_ch,
                        bool calibration_flag);

uint32_t adc_raw_measure(ADC_HandleTypeDef* hadc);
#endif  // !ADC_CONTROL_H

#ifndef ADC_CONTROL_H
#define ADC_CONTROL_H
#include <stdio.h>
#include "main.h"

#define NUM_CHANNELS 4

#define CHANNEL_NUM_SAMPLES 10

typedef struct {
    _Bool channel[NUM_CHANNELS];
    _Bool channel_unapplied[NUM_CHANNELS];
    _Bool applied;
    uint32_t avg_last_measure[NUM_CHANNELS];
    unsigned int pin[NUM_CHANNELS];
    unsigned int count_active;
    ADC_HandleTypeDef* hadc;
} adc_channels;

adc_channels* adc_create_channel_struct(ADC_HandleTypeDef* hadc);

void adc_flip_unapplied_channel(adc_channels* adc_ch, size_t channel);

void adc_remove_unapplied_channels(adc_channels* adc_ch);

size_t adc_count_active_channels(adc_channels* adc_ch);

void adc_apply_channels(adc_channels* adc_ch);

void adc_set_rank(ADC_ChannelConfTypeDef* sConfig, size_t rank);

void adc_set_channel(ADC_ChannelConfTypeDef* sConfig, size_t channel);

void adc_init_hal_conversion(ADC_HandleTypeDef* hadc,
                             const uint32_t n_conversion);

void adc_realloc_v_measures(adc_channels* adc_ch, uint32_t** measures);

void adc_setup_channel_struct(ADC_HandleTypeDef* hadc,
                              adc_channels* adc_ch,
                              _Bool calibration_flag);

void adc_make_voltage_avg(uint32_t* buff,
                          adc_channels* adc_ch,
                          const uint32_t* measures);

uint32_t adc_raw_measure(ADC_HandleTypeDef* hadc);

uint32_t adc_get_voltage(uint32_t measure);

uint32_t adc_get_v_ref(void);
#endif  // !ADC_CONTROL_H

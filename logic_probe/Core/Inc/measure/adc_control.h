#ifndef ADC_CONTROL_H
#define ADC_CONTROL_H
#include <stdio.h>
#include "main.h"
#include "my_bool.h"

#define NUM_CHANNELS 4

#define CHANNEL_NUM_SAMPLES 10

typedef struct {
    bool channel[NUM_CHANNELS];
    bool channel_unapplied[NUM_CHANNELS];
    bool applied;
    uint32_t avg_last_measure[NUM_CHANNELS];
    unsigned int pin[NUM_CHANNELS];
    unsigned int count_active;
    ADC_HandleTypeDef* hadc;
} adc_channels;

adc_channels* create_adc_channels(ADC_HandleTypeDef* hadc);

void flip_adc_unapplied_channel(adc_channels* adc_ch, size_t channel);

void adc_remove_unapplied_channels(adc_channels* adc_ch);

size_t count_active_channels(adc_channels* adc_ch);

void adc_apply_channels(adc_channels* adc_ch);

void set_adc_rank(ADC_ChannelConfTypeDef* sConfig, size_t rank);

void set_adc_channel(ADC_ChannelConfTypeDef* sConfig, size_t channel);

void adc_init_hal_conversion(ADC_HandleTypeDef* hadc,
                             const uint32_t n_conversion);

void realloc_v_measures(adc_channels* adc_ch, uint32_t** measures);

void setup_adc_channels(ADC_HandleTypeDef* hadc,
                        adc_channels* adc_ch,
                        bool calibration_flag);

void adc_make_avg(uint32_t* buff,
                  adc_channels* adc_ch,
                  const uint32_t* measures);

uint32_t adc_raw_measure(ADC_HandleTypeDef* hadc);

uint32_t get_voltage(uint32_t measure);
#endif  // !ADC_CONTROL_H

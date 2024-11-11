#include "adc_control.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "my_error_handle.h"

#define ADC_TIMEOUT 100

extern uint32_t v_ref;

adc_channels* create_adc_channels(ADC_HandleTypeDef* hadc) {
    adc_channels* adc_ch = (adc_channels*)malloc(sizeof(adc_channels));

    if (adc_ch == NULL) {
        exception("Failed to allocate adc_channels struct");
    }

    for (size_t i = 0; i < NUM_CHANNELS; ++i) {
        adc_ch->channel[i] = false;
    }

    unsigned int pin_values[] = {0, 1, 4, 5};  // todo: lepe?
    memcpy(adc_ch->pin, pin_values, sizeof(pin_values));

    adc_ch->applied = true;
    adc_ch->count_active = count_active_channels(adc_ch);
    adc_ch->hadc = hadc;

    return adc_ch;
}

void flip_adc_channel(adc_channels* adc_ch, size_t channel) {
    if (channel == 0) {
        // todo: error
        return;
    }

    channel -= 1;
    adc_ch->channel[channel] = !adc_ch->channel[channel];
    adc_ch->count_active = count_active_channels(adc_ch);
    adc_ch->applied = false;
}

size_t count_active_channels(adc_channels* adc_ch) {
    size_t count = 0;
    for (size_t i = 0; i < NUM_CHANNELS; ++i) {
        if (adc_ch->channel[i]) {
            ++count;
        }
    }
    return count;
}

void set_adc_rank(ADC_ChannelConfTypeDef* sConfig, size_t rank) {
    static const uint32_t rank_map[] = {ADC_REGULAR_RANK_1, ADC_REGULAR_RANK_2,
                                        ADC_REGULAR_RANK_3, ADC_REGULAR_RANK_4};

    if (rank >= 1 && rank <= 4) {
        sConfig->Rank = rank_map[rank];
    }
}

void set_adc_channel(ADC_ChannelConfTypeDef* sConfig, size_t channel) {
    static const uint32_t channel_map[] = {ADC_CHANNEL_0, ADC_CHANNEL_1,
                                           ADC_CHANNEL_4, ADC_CHANNEL_5};
    if (channel >= 1 && channel <= 4) {
        sConfig->Channel = channel_map[channel];
    }
}

void adc_init_hal_conversion(ADC_HandleTypeDef* hadc,
                             const uint32_t n_conversion) {
    if (n_conversion > 0 && n_conversion <= 4) {
        hadc->Init.NbrOfConversion = n_conversion;

        if (HAL_ADC_Init(hadc) != HAL_OK) {
            exception("Failed to init ADC");
        }

    } else {
        exception("Bad input in adc_init_hal_conversion");
    }
}

void setup_adc_channels(ADC_HandleTypeDef* hadc,
                        adc_channels* adc_ch,
                        bool calibration_flag) {
    // has to be deinit
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
    unsigned int active_channel = 0;

    if (adc_ch->count_active == 0 || calibration_flag) {
        // setup refint
        adc_init_hal_conversion(hadc, 1);

        sConfig.Rank = ADC_REGULAR_RANK_1;
        sConfig.Channel = ADC_CHANNEL_VREFINT;
        if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK) {
            exception("Failed to setup ADC_CONFIG");
        }
        return;
    }

    // hal Init
    adc_init_hal_conversion(hadc, adc_ch->count_active);

    for (size_t i = 0; i < NUM_CHANNELS; ++i) {
        if (active_channel == adc_ch->count_active) {
            break;
        }
        if (adc_ch->channel[i] && adc_ch->applied) {
            set_adc_rank(&sConfig, active_channel++);
            set_adc_channel(&sConfig, i);

            if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK) {
                exception("Failed to setup ADC_CONFIG in forloop");
            }
        }
    }
}

uint32_t adc_raw_measure(ADC_HandleTypeDef* hadc) {
    uint32_t raw_voltage_value;

    if (HAL_ADC_PollForConversion(hadc, ADC_TIMEOUT) != HAL_OK) {
        exception("Failed to get raw adc value");
    }
    raw_voltage_value = HAL_ADC_GetValue(hadc);
    return raw_voltage_value;
}

uint32_t get_voltage(uint32_t measure) {
    return __HAL_ADC_CALC_DATA_TO_VOLTAGE(v_ref, measure, ADC_RESOLUTION_12B);
}

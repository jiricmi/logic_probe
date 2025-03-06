#include "adc_control.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "my_error_handle.h"

#define ADC_TIMEOUT 100

extern uint32_t v_ref;
extern uint32_t* v_measures;
extern ADC_HandleTypeDef hadc1;

adc_channels* adc_create_channel_struct(ADC_HandleTypeDef* hadc) {
    adc_channels* adc_ch = (adc_channels*)malloc(sizeof(adc_channels));

    if (adc_ch == NULL) {
        exception("Failed to allocate adc_channels struct");
    }

    adc_ch->channel[0] = true;
    for (size_t i = 1; i < NUM_CHANNELS; ++i) {
        adc_ch->channel[i] = false;
    }

    for (size_t i = 0; i < NUM_CHANNELS; ++i) {
        adc_ch->channel_unapplied[i] = adc_ch->channel[i];
    }

    unsigned int pin_values[] = {0, 1, 4, 5};  // todo: lepe?
    memcpy(adc_ch->pin, pin_values, sizeof(pin_values));

    for (size_t i = 0; i < NUM_CHANNELS; ++i) {
        adc_ch->avg_last_measure[0] = 0;
    }

    adc_ch->applied = true;
    adc_ch->count_active = adc_count_active_channels(adc_ch);
    adc_ch->hadc = hadc;

    return adc_ch;
}

void adc_flip_unapplied_channel(adc_channels* adc_ch, size_t channel) {
    if (channel == 0) {
        return;
    }

    channel -= 1;
    adc_ch->channel_unapplied[channel] = !adc_ch->channel_unapplied[channel];
    adc_ch->applied = false;
}

void adc_remove_unapplied_channels(adc_channels* adc_ch) {
    for (size_t i = 0; i < NUM_CHANNELS; ++i) {
        adc_ch->channel_unapplied[i] = adc_ch->channel[i];
    }
    adc_ch->applied = true;
}

size_t adc_count_active_channels(adc_channels* adc_ch) {
    size_t count = 0;
    for (size_t i = 0; i < NUM_CHANNELS; ++i) {
        if (adc_ch->channel[i]) {
            ++count;
        }
    }
    return count;
}

void adc_apply_channels(adc_channels* adc_ch) {
    HAL_ADC_Stop_DMA(&hadc1);
    for (size_t i = 0; i < NUM_CHANNELS; ++i) {
        adc_ch->channel[i] = adc_ch->channel_unapplied[i];
    }
    adc_ch->count_active = adc_count_active_channels(adc_ch);
    adc_ch->applied = true;
    adc_realloc_v_measures(adc_ch, &v_measures);
}

void adc_set_rank(ADC_ChannelConfTypeDef* sConfig, size_t rank) {
    static const uint32_t rank_map[] = {ADC_REGULAR_RANK_1, ADC_REGULAR_RANK_2,
                                        ADC_REGULAR_RANK_3, ADC_REGULAR_RANK_4};

    if (rank >= 1 && rank <= 4) {
        sConfig->Rank = rank_map[rank];
    }
}

void adc_set_channel(ADC_ChannelConfTypeDef* sConfig, size_t channel) {
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

void adc_realloc_v_measures(adc_channels* adc_ch, uint32_t** measures) {
    size_t measure_size;

    if (adc_ch->count_active == 0) {
        measure_size = 1;
    } else {
        measure_size = (size_t)adc_ch->count_active * CHANNEL_NUM_SAMPLES;
    }

    if (*measures == NULL) {
        *measures = (uint32_t*)calloc(measure_size, sizeof(uint32_t));
    } else {
        uint32_t* new_measures =
            (uint32_t*)realloc(*measures, measure_size * sizeof(uint32_t));
        *measures = new_measures;
    }
    if (*measures == NULL) {
        exception("Failed to allocate memory for v_measures");
    }
}

void adc_setup_channel_struct(ADC_HandleTypeDef* hadc,
                              adc_channels* adc_ch,
                              _Bool calibration_flag) {
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
            adc_set_rank(&sConfig, active_channel++);
            adc_set_channel(&sConfig, i);

            if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK) {
                exception("Failed to setup ADC_CONFIG in forloop");
            }
        }
    }
}

void adc_make_voltage_avg(uint32_t* buff,
                          adc_channels* adc_ch,
                          const uint32_t* measures) {
    if (adc_ch->count_active == 0) {
        return;
    }

    uint64_t avgs[NUM_CHANNELS];

    for (size_t i = 0; i < NUM_CHANNELS; ++i) {
        avgs[i] = 0;
    }

    for (size_t i = 0; i < adc_ch->count_active * CHANNEL_NUM_SAMPLES; ++i) {
        avgs[i % adc_ch->count_active] += measures[i];
    }

    for (size_t i = 0; i < adc_ch->count_active; ++i) {
        avgs[i] /= CHANNEL_NUM_SAMPLES;
    }

    uint32_t ordered_buff[NUM_CHANNELS];

    size_t index = 0;
    for (size_t i = 0; i < NUM_CHANNELS; ++i) {
        if (adc_ch->channel[i]) {
            ordered_buff[i] = (uint32_t)avgs[index++];
        } else {
            ordered_buff[i] = 0;
        }
    }

    for (size_t i = 0; i < NUM_CHANNELS; ++i) {
        buff[i] = (ordered_buff[i] + adc_ch->avg_last_measure[i]) / 2;
        adc_ch->avg_last_measure[i] = ordered_buff[i];
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

uint32_t adc_get_voltage(uint32_t measure) {
    return __HAL_ADC_CALC_DATA_TO_VOLTAGE(v_ref, measure, ADC_RESOLUTION_12B);
}

uint32_t adc_get_v_ref(void) {
    // HAL_ADC_Start(&hadc1);
    uint32_t raw_voltage_value = adc_raw_measure(&hadc1);
    return __HAL_ADC_CALC_VREFANALOG_VOLTAGE(raw_voltage_value,
                                             ADC_RESOLUTION_12B);
}

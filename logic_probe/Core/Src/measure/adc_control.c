#include "adc_control.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "my_error_handle.h"

adc_vars_t* adc_create_channel_struct(ADC_HandleTypeDef* hadc) {
    adc_vars_t* adc_ch = (adc_vars_t*)malloc(sizeof(adc_vars_t));

    if (adc_ch == NULL) {
        exception("Failed to allocate adc_vars_t struct");
    }

    adc_ch->channel_state[0] = true;
    adc_ch->channel_state[1] = true;
    for (size_t i = 2; i < NUM_CHANNELS; ++i) {
        adc_ch->channel_state[i] = false;
    }

    memcpy(adc_ch->channel_state_unapplied, adc_ch->channel_state,
           sizeof(adc_ch->channel_state));

    memset(adc_ch->avg_voltage_current, 0, sizeof(adc_ch->avg_voltage_current));
    memset(adc_ch->avg_voltage_previous, 0,
           sizeof(adc_ch->avg_voltage_previous));

    adc_ch->applied = true;

    unsigned int pin_values[] = {0, 0, 1, 4, 5};
    memcpy(adc_ch->pin, pin_values, sizeof(pin_values));

    adc_ch->n_active_channels = adc_count_active_channels(adc_ch);

    adc_ch->hadc = hadc;

    adc_ch->voltage_measures = (uint32_t*)malloc(
        sizeof(uint32_t) * CHANNEL_NUM_SAMPLES * adc_ch->n_active_channels);

    if (adc_ch->voltage_measures == NULL) {
        exception("cannot malloc voltage measures");
    }
    return adc_ch;
}

void adc_flip_unapplied_channel(adc_vars_t* adc_ch, size_t channel) {
    if (channel == 0) {
        return;
    }

    adc_ch->channel_state_unapplied[channel] =
        !adc_ch->channel_state_unapplied[channel];
    adc_ch->applied = false;
}

void adc_remove_unapplied_channels(adc_vars_t* adc_ch) {
    for (size_t i = 1; i < NUM_CHANNELS; ++i) {
        adc_ch->channel_state_unapplied[i] = adc_ch->channel_state[i];
    }
    adc_ch->applied = true;
}

uint8_t adc_count_active_channels(adc_vars_t* adc_ch) {
    size_t count = 0;
    for (size_t i = 0; i < NUM_CHANNELS; ++i) {
        if (adc_ch->channel_state[i]) {
            ++count;
        }
    }
    return count;
}

void adc_apply_channels(adc_vars_t* adc_ch) {
    HAL_ADC_Stop_DMA(adc_ch->hadc);
    for (size_t i = 1; i < NUM_CHANNELS; ++i) {
        adc_ch->channel_state[i] = adc_ch->channel_state_unapplied[i];
    }
    adc_ch->n_active_channels = adc_count_active_channels(adc_ch);
    adc_realloc_v_measures(adc_ch);
    adc_ch->applied = true;
}

void adc_set_rank(ADC_ChannelConfTypeDef* sConfig, uint8_t rank) {
    static const uint32_t rank_map[] = {ADC_REGULAR_RANK_1, ADC_REGULAR_RANK_2,
                                        ADC_REGULAR_RANK_3, ADC_REGULAR_RANK_4,
                                        ADC_REGULAR_RANK_5};

    if (rank <= 5) {
        sConfig->Rank = rank_map[rank];
    }
}

void adc_set_channel(ADC_ChannelConfTypeDef* sConfig, uint8_t channel) {
    static const uint32_t channel_map[] = {ADC_CHANNEL_VREFINT, ADC_CHANNEL_0,
                                           ADC_CHANNEL_1, ADC_CHANNEL_4,
                                           ADC_CHANNEL_5};
    if (channel <= 5) {
        sConfig->Channel = channel_map[channel];
    }
}

void adc_init_hal_conversion(ADC_HandleTypeDef* hadc,
                             const uint32_t n_conversion) {
    if (n_conversion >= 1 && n_conversion <= 5) {
        hadc->Init.NbrOfConversion = n_conversion;

        if (HAL_ADC_Init(hadc) != HAL_OK) {
            exception("Failed to init ADC");
        }

    } else {
        exception("Bad input in adc_init_hal_conversion");
    }
}

void adc_realloc_v_measures(adc_vars_t* adc_ch) {
    size_t measure_size;

    measure_size = (size_t)(adc_ch->n_active_channels) * CHANNEL_NUM_SAMPLES;

    if (adc_ch->voltage_measures == NULL) {
        adc_ch->voltage_measures =
            (uint32_t*)calloc(measure_size, sizeof(uint32_t));
    } else {
        uint32_t* new_measures = (uint32_t*)realloc(
            adc_ch->voltage_measures, measure_size * sizeof(uint32_t));
        adc_ch->voltage_measures = new_measures;
    }
    if (adc_ch->voltage_measures == NULL) {
        exception("Failed to allocate memory for v_measures");
    }
}

void adc_setup_channel_struct(adc_vars_t* adc_ch) {
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
    unsigned int active_channel = 0;

    adc_init_hal_conversion(adc_ch->hadc, adc_ch->n_active_channels);

    for (size_t i = 0; i < NUM_CHANNELS; ++i) {
        if (active_channel == adc_ch->n_active_channels) {
            break;
        }
        if (adc_ch->channel_state[i]) {
            adc_set_rank(&sConfig, active_channel++);
            adc_set_channel(&sConfig, i);

            if (HAL_ADC_ConfigChannel(adc_ch->hadc, &sConfig) != HAL_OK) {
                exception("Failed to setup ADC_CONFIG in forloop");
            }
        }
    }
}

// todo: udelat pri mereni
void adc_get_avg_voltages(adc_vars_t* adc_ch) {
    uint64_t avgs[NUM_CHANNELS];
    memset(avgs, 0, sizeof(avgs));

    for (size_t i = 0; i < adc_ch->n_active_channels * CHANNEL_NUM_SAMPLES;
         ++i) {
        avgs[i % adc_ch->n_active_channels] += adc_ch->voltage_measures[i];
    }

    for (size_t i = 0; i < adc_ch->n_active_channels; ++i) {
        avgs[i] /= CHANNEL_NUM_SAMPLES;
    }

    uint32_t ordered_buff[NUM_CHANNELS];

    size_t index = 0;
    for (size_t i = 0; i < NUM_CHANNELS; ++i) {
        if (adc_ch->channel_state[i]) {
            ordered_buff[i] = (uint32_t)avgs[index++];
        } else {
            ordered_buff[i] = 0;
        }
    }

    memcpy(adc_ch->avg_voltage_previous, adc_ch->avg_voltage_current,
           sizeof(adc_ch->avg_voltage_previous));

    memcpy(adc_ch->avg_voltage_current, ordered_buff,
           sizeof(adc_ch->avg_voltage_current));
}

void adc_calculate_floating_voltage_avg(uint32_t* floating_avg_measures,
                                        const adc_vars_t* adc_ch) {
    for (uint8_t i = 0; i < NUM_CHANNELS; ++i) {
        floating_avg_measures[i] =
            (adc_ch->avg_voltage_previous[i] + adc_ch->avg_voltage_current[i]) /
            2;
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

uint32_t adc_get_voltage(uint32_t v_ref, uint32_t measure) {
    return __HAL_ADC_CALC_DATA_TO_VOLTAGE(v_ref, measure, ADC_RESOLUTION_12B);
}

uint32_t adc_get_v_ref(uint32_t raw_voltage_value) {
    return __HAL_ADC_CALC_VREFANALOG_VOLTAGE(raw_voltage_value,
                                             ADC_RESOLUTION_12B);
}

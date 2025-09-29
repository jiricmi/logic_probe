#include "adc_control.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "global_vars.h"
#include "my_error_handle.h"

extern global_vars_t global_var;

adc_vars_t* adc_create_channel_struct(ADC_HandleTypeDef* hadc,
                                      TIM_HandleTypeDef* timer) {
    adc_vars_t* adc_ch = (adc_vars_t*)malloc(sizeof(adc_vars_t));
    adc_ch->timer = timer;
    adc_ch->adc_num_channels =
        (global_var.can_advanced) ? ADC_NUM_CHANNELS_20 : ADC_NUM_CHANNELS_8;

    if (adc_ch == NULL) {
        exception("Failed to allocate adc_vars_t struct");
    }

    adc_ch->channel_state[ADC_REF_INDEX - 1] = true;
    adc_ch->channel_state[ADC_CH_PERMANENT_INDEX - 1] = true;
    for (size_t i = ADC_CH_PERMANENT_INDEX; i < adc_ch->adc_num_channels; ++i) {
        adc_ch->channel_state[i] = false;
    }

    memcpy(adc_ch->channel_state_unapplied, adc_ch->channel_state,
           sizeof(adc_ch->channel_state));

    memset(adc_ch->avg_voltage, 0, sizeof(adc_ch->avg_voltage));

    adc_ch->applied = true;
    adc_ch->base_resistor = ADC_BASE_RESISTOR;

    uint8_t pin_values[] = PIN_VALS;
    memcpy(adc_ch->pin, pin_values, sizeof(pin_values));
    char gpio_pins[] = PIN_GPIO;
    memcpy(adc_ch->gpio_pin, gpio_pins, sizeof(gpio_pins));

    if (global_var.can_advanced) {
        uint8_t pin_real[] = PIN_REAL;
        memcpy(adc_ch->pin_real, pin_real, sizeof(pin_real));
    } else {
        uint8_t pin_real[] = PIN_REAL_8;
        memcpy(adc_ch->pin_real, pin_real, sizeof(pin_real));
    }

    char gpio_nucleo[] = PIN_NUCLEO;
    memcpy(adc_ch->gpio_nucleo, gpio_nucleo, sizeof(gpio_nucleo));

    adc_ch->n_active_channels = adc_count_active_channels(adc_ch);

    adc_ch->hadc = hadc;

    adc_ch->voltage_measures = (uint32_t*)malloc(
        sizeof(uint32_t) * CHANNEL_NUM_SAMPLES * adc_ch->n_active_channels);

    if (adc_ch->voltage_measures == NULL) {
        exception("cannot malloc voltage measures");
    }
    adc_ch->measures_index = 0;
    return adc_ch;
}

void adc_start_measure(adc_vars_t* adc_ch) {
    __HAL_TIM_SET_AUTORELOAD(adc_ch->timer, 7);
    adc_ch->measures_index = 0;
    HAL_TIM_Base_Start_IT(adc_ch->timer);
}

void adc_stop_measure(adc_vars_t* adc_ch) {
    HAL_TIM_Base_Stop_IT(adc_ch->timer);
    // stop timer
}

void adc_flip_unapplied_channel(adc_vars_t* adc_ch, size_t channel) {
    if (channel == ADC_REF_INDEX - 1) {
        return;
    }

    if (channel >= adc_ch->adc_num_channels) {
        return;
    }

    adc_ch->channel_state_unapplied[channel] =
        !adc_ch->channel_state_unapplied[channel];
    adc_ch->applied = false;
}

void adc_remove_unapplied_channels(adc_vars_t* adc_ch) {
    for (size_t i = ADC_REF_INDEX; i < adc_ch->adc_num_channels; ++i) {
        adc_ch->channel_state_unapplied[i] = adc_ch->channel_state[i];
    }
    adc_ch->applied = true;
}

uint8_t adc_count_active_channels(adc_vars_t* adc_ch) {
    size_t count = 0;
    for (size_t i = 0; i < adc_ch->adc_num_channels; ++i) {
        if (adc_ch->channel_state[i]) {
            ++count;
        }
    }
    return count;
}

void adc_apply_channels(adc_vars_t* adc_ch) {
    adc_stop_measure(adc_ch);
    for (size_t i = ADC_REF_INDEX; i < adc_ch->adc_num_channels; ++i) {
        adc_ch->channel_state[i] = adc_ch->channel_state_unapplied[i];
    }
    adc_ch->n_active_channels = adc_count_active_channels(adc_ch);
    adc_realloc_v_measures(adc_ch);
    adc_ch->applied = true;
    adc_ch->measures_index = 0;
}

void adc_set_rank(ADC_ChannelConfTypeDef* sConfig, uint8_t rank) {
    static const uint32_t rank_map[] = RANK_MAP;

    if (rank <= global_var.adc_vars->adc_num_channels) {
        sConfig->Rank = rank_map[rank];
    }
}

void adc_set_channel(ADC_ChannelConfTypeDef* sConfig, uint8_t channel) {
    static const uint32_t channel_map[] = CHANNEL_MAP;
    if (channel <= global_var.adc_vars->adc_num_channels) {
        sConfig->Channel = channel_map[channel];
    }
}

void adc_init_hal_conversion(ADC_HandleTypeDef* hadc,
                             const uint32_t n_conversion) {
    if (n_conversion >= ADC_REF_INDEX &&
        n_conversion <= global_var.adc_vars->adc_num_channels) {
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

    for (size_t i = 0; i < adc_ch->adc_num_channels; ++i) {
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
    uint64_t avgs[ADC_NUM_CHANNELS_20];
    memset(avgs, 0, sizeof(avgs));

    for (size_t i = 0; i < adc_ch->n_active_channels * CHANNEL_NUM_SAMPLES;
         ++i) {
        avgs[i % adc_ch->n_active_channels] += adc_ch->voltage_measures[i];
    }

    for (size_t i = 0; i < adc_ch->n_active_channels; ++i) {
        avgs[i] /= CHANNEL_NUM_SAMPLES;
    }

    uint32_t ordered_buff[ADC_NUM_CHANNELS_20];

    size_t index = 0;
    for (size_t i = 0; i < ADC_NUM_CHANNELS_20; ++i) {
        if (adc_ch->channel_state[i]) {
            ordered_buff[i] = (uint32_t)avgs[index++];
        } else {
            ordered_buff[i] = 0;
        }
    }

    memcpy(adc_ch->avg_voltage, ordered_buff, sizeof(adc_ch->avg_voltage));
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

probe_state_t adc_local_logic_probe(adc_vars_t* adc_ch,
                                    local_substate_t state) {
    uint32_t ref_voltage = adc_get_v_ref(adc_ch->avg_voltage[0]);

    uint8_t index = (state == LOCAL_SUBSTATE_CHANNEL_1) ? 1 : 2;

    uint32_t measured_voltage =
        adc_get_voltage(ref_voltage, adc_ch->avg_voltage[index]);

    if (measured_voltage > HIGH_MIN_V) {
        return PROBE_STATE_HIGH;
    } else if (measured_voltage < LOW_MAX_V) {
        return PROBE_STATE_LOW;
    }
    return PROBE_STATE_UNDEFINED;
}

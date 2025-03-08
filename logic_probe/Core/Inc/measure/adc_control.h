#ifndef ADC_CONTROL_H
#define ADC_CONTROL_H
#include <stdint.h>
#include <stdio.h>
#include "main.h"

#define NUM_CHANNELS 5           // Number of ADC channels
#define CHANNEL_NUM_SAMPLES 100  // Number of samples per channel for averaging
#define V_REF_DEFAULT 3300       // Default reference voltage in millivolts
#define ADC_TIMEOUT 100

typedef struct {
    _Bool channel_state[NUM_CHANNELS];
    _Bool channel_state_unapplied[NUM_CHANNELS];
    _Bool applied;
    _Bool resistance_mode;
    uint32_t base_resistor;
    uint32_t avg_voltage_current[NUM_CHANNELS];
    uint32_t avg_voltage_previous[NUM_CHANNELS];
    uint32_t pin[NUM_CHANNELS];
    uint8_t n_active_channels;
    ADC_HandleTypeDef* hadc;
    uint32_t* voltage_measures;
} adc_vars_t;

/**
 * @brief Creates and initializes an ADC channel structure.
 *
 * @param hadc Pointer to the ADC handle.
 * @return Pointer to the created adc_vars_t structure.
 */
adc_vars_t* adc_create_channel_struct(ADC_HandleTypeDef* hadc);

/**
 * @brief Toggles the state of an unapplied ADC channel.
 *
 * @param adc_ch Pointer to the ADC variables structure.
 * @param channel Channel index to toggle.
 */
void adc_flip_unapplied_channel(adc_vars_t* adc_ch, size_t channel);

/**
 * @brief Resets unapplied channel states to match the current states.
 *
 * @param adc_ch Pointer to the ADC variables structure.
 */
void adc_remove_unapplied_channels(adc_vars_t* adc_ch);

/**
 * @brief Counts the number of active ADC channels.
 *
 * @param adc_ch Pointer to the ADC variables structure.
 * @return Number of active channels.
 */
uint8_t adc_count_active_channels(adc_vars_t* adc_ch);

/**
 * @brief Applies unapplied channel states to the ADC configuration.
 *
 * @param adc_ch Pointer to the ADC variables structure.
 */
void adc_apply_channels(adc_vars_t* adc_ch);

/**
 * @brief Sets the rank for an ADC channel configuration.
 *
 * @param sConfig Pointer to the ADC channel configuration structure.
 * @param rank Rank to set.
 */
void adc_set_rank(ADC_ChannelConfTypeDef* sConfig, uint8_t rank);

/**
 * @brief Sets the channel for an ADC channel configuration.
 *
 * @param sConfig Pointer to the ADC channel configuration structure.
 * @param channel Channel to set.
 */
void adc_set_channel(ADC_ChannelConfTypeDef* sConfig, uint8_t channel);

/**
 * @brief Initializes the HAL ADC conversion settings.
 *
 * @param hadc Pointer to the ADC handle.
 * @param n_conversion Number of conversions to configure.
 */
void adc_init_hal_conversion(ADC_HandleTypeDef* hadc,
                             const uint32_t n_conversion);

/**
 * @brief Reallocates memory for voltage measurements based on active channels.
 *
 * @param adc_ch Pointer to the ADC variables structure.
 */
void adc_realloc_v_measures(adc_vars_t* adc_ch);

/**
 * @brief Sets up the ADC channel structure and configures the ADC.
 *
 * @param adc_ch Pointer to the ADC variables structure.
 */
void adc_setup_channel_struct(adc_vars_t* adc_ch);

/**
 * @brief Calculates the average voltages for all active channels.
 *
 * @param adc_ch Pointer to the ADC variables structure.
 */
void adc_get_avg_voltages(adc_vars_t* adc_ch);

/**
 * @brief Calculates the floating average voltage for all channels.
 *
 * @param floating_avg_measures Array to store the floating average voltages.
 * @param adc_ch Pointer to the ADC variables structure.
 */
void adc_calculate_floating_voltage_avg(uint32_t* floating_avg_measures,
                                        const adc_vars_t* adc_ch);

/**
 * @brief Performs a raw ADC measurement.
 *
 * @param hadc Pointer to the ADC handle.
 * @return Raw ADC measurement value.
 */
uint32_t adc_raw_measure(ADC_HandleTypeDef* hadc);

/**
 * @brief Converts a raw ADC measurement to voltage.
 *
 * @param v_ref Reference voltage in millivolts.
 * @param measure Raw ADC measurement value.
 * @return Voltage in millivolts.
 */
uint32_t adc_get_voltage(uint32_t v_ref, uint32_t measure);

/**
 * @brief Calculates the reference voltage from a raw ADC measurement.
 *
 * @param raw_voltage_value Raw ADC measurement value.
 * @return Reference voltage in millivolts.
 */
uint32_t adc_get_v_ref(uint32_t raw_voltage_value);

#endif  // ADC_CONTROL_H

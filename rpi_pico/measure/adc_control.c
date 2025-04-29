#include "adc_control.h"
#include <hardware/adc.h>
#include <pico/time.h>
#include <stdio.h>
#include <string.h>
#include "global_vars.h"

extern global_vars_t global_var;

void adc_init_struct(adc_vars_t* perif) {
    memset(perif, 0, sizeof(*perif));

    perif->base_resistor = ADC_BASE_RESISTOR;

    const uint8_t gpio_pio[] = ADC_PINS;
    memcpy(perif->pinout, gpio_pio, ADC_NUM_CHANNELS);
    for (uint8_t channel = 0; channel < ADC_NUM_CHANNELS; ++channel) {
        adc_gpio_init(gpio_pio[channel]);
    }
}

bool adc_measure_callback(repeating_timer_t* rt) {
    adc_vars_t* perif = &global_var.adc_perif;
    for (uint8_t channel = 0; channel < ADC_NUM_CHANNELS; ++channel) {
        adc_select_input(channel);
        perif->voltage_measures[channel][perif->measures_index] = adc_read();
    }
    perif->measures_index = (perif->measures_index + 1) % ADC_CHANNEL_SAMPLES;
    return true;
}

void adc_start_measure(adc_vars_t* perif) {
    if (!add_repeating_timer_ms(ADC_DELAY_MEASURE_MS, adc_measure_callback,
                                NULL, &perif->timer)) {
        printf("Error during ADC timer init\n");
    }
}

void adc_stop_measure(adc_vars_t* perif) {
    cancel_repeating_timer(&perif->timer);
}

uint32_t adc_get_voltage(const uint32_t raw_voltage) {
    return raw_voltage * 3300 / 4095;
}

void adc_get_avg_voltages(adc_vars_t* perif) {
    for (uint8_t channel = 0; channel < ADC_NUM_CHANNELS; ++channel) {
        uint64_t sum = 0;

        for (uint8_t index = 0; index < ADC_CHANNEL_SAMPLES; ++index) {
            sum += perif->voltage_measures[channel][index];
        }

        perif->avg_voltage[channel] = (float)sum / (float)ADC_CHANNEL_SAMPLES;
    }
}

#ifndef ADC_CONTROL_H
#define ADC_CONTROL_H
#include <pico/time.h>
#include <stdint.h>

#define ADC_CHANNEL_SAMPLES 50
#define ADC_NUM_CHANNELS 3
#define ADC_BASE_RESISTOR 10000
#define ADC_DELAY_MEASURE_MS 10
#define LOW_MAX_V 800
#define HIGH_MIN_V 2000
#define ADC_PINS {26, 27, 28}

typedef struct {
    // _Bool resistance_mode
    repeating_timer_t timer;
    uint32_t base_resistor;
    uint32_t avg_voltage[ADC_NUM_CHANNELS];
    uint8_t pinout[ADC_NUM_CHANNELS];
    uint32_t voltage_measures[ADC_NUM_CHANNELS][ADC_CHANNEL_SAMPLES];
    uint8_t measures_index;
} adc_vars_t;

void adc_init_struct(adc_vars_t* perif);

bool adc_measure_callback(repeating_timer_t* rt);

void adc_start_measure(adc_vars_t* perif);

void adc_stop_measure(adc_vars_t* perif);

uint32_t adc_get_voltage(const uint32_t raw_voltage);

void adc_get_avg_voltages(adc_vars_t* perif);

#endif

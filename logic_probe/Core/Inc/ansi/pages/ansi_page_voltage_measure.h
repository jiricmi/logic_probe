#ifndef ANSI_PAGE_VOLTAGE_MEASURE_H
#define ANSI_PAGE_VOLTAGE_MEASURE_H

#include <stdint.h>
#include "adc_control.h"

#define ANSI_VOLTAGE_TEXT_BUFFER 50
#define LOW_MAX_V 800
#define HIGH_MIN_V 2000
#define ADC_MEASURE_ROW 13
#define ADC_MEASURE_CENTER ((TERMINAL_WIDTH / 2) - 9)
#define ADC_FLOATING_POINT 3

#define VOLTAGE_TEXT_OFFSET 7
#define BASE_RESISTOR_OFFSET 11
#define RESISTANCE_OFFSET 9

void ansi_render_voltage_page(void);

void ansi_render_resistance_measure(const adc_vars_t* adc_ch);

void ansi_render_resistor_schema(const uint8_t row,
                                 const uint8_t col,
                                 const uint32_t volt_ref,
                                 const uint32_t base_resistor,
                                 const uint32_t resistance);

void ansi_render_resistance_circuit(const uint8_t row, const uint8_t col);

void ansi_render_resistance_values(const uint8_t row,
                                   const uint8_t col,
                                   const uint32_t volt_ref,
                                   const uint32_t base_resistor,
                                   const uint32_t resistance);

void ansi_render_voltage_measures(const adc_vars_t* adc_ch);

void ansi_render_channel_voltage(uint32_t voltage,
                                 uint8_t channel,
                                 uint32_t pin,
                                 uint32_t floating_point);

void ansi_render_state_channel(const uint8_t row,
                               const uint8_t col,
                               const uint8_t channel,
                               const adc_vars_t* adc_ch);

void ansi_render_adc_change_message(const uint8_t row,
                                    const uint8_t col,
                                    const adc_vars_t* adc_ch);

void ansi_render_reference_voltage(uint32_t v_ref, uint32_t floating_point);

void ansi_render_logic_probe_ch(uint32_t voltage, _Bool channel);

#endif

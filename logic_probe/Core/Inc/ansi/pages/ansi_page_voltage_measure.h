#ifndef ANSI_PAGE_VOLTAGE_MEASURE_H
#define ANSI_PAGE_VOLTAGE_MEASURE_H

#include <stdint.h>
#include "adc_control.h"

#define ANSI_VOLTAGE_TEXT_BUFFER 50
#define LOW_MAX_V 800
#define HIGH_MIN_V 2000

void ansi_render_voltage_page(void);

void ansi_render_voltage_measures(const adc_vars_t* adc_ch);

void ansi_render_channel_voltage(uint32_t voltage,
                                 unsigned int channel,
                                 unsigned int pin,
                                 unsigned int floating_point);

void ansi_render_state_channel(const uint8_t row,
                               const uint8_t col,
                               const uint8_t channel,
                               const adc_vars_t* adc_ch);

void ansi_render_adc_change_message(const uint8_t row,
                                    const uint8_t col,
                                    const adc_vars_t* adc_ch);

void ansi_render_reference_voltage(uint32_t v_ref, unsigned int floating_point);

void ansi_render_logic_probe_ch(uint32_t voltage, _Bool channel);

#endif

#ifndef ANSI_PAGE_VOLTAGE_MEASURE_H
#define ANSI_PAGE_VOLTAGE_MEASURE_H

#include <stdint.h>
#include "adc_control.h"

void ansi_render_voltage_page(void);

void ansi_render_voltage_measures(const uint32_t v_ref,
                                  const uint32_t* measures,
                                  adc_channels* adc_ch);

void ansi_render_channel_voltage(uint32_t voltage,
                                 unsigned int channel,
                                 unsigned int pin,
                                 unsigned int floating_point);

void ansi_render_reference_voltage(uint32_t v_ref, unsigned int floating_point);

void ansi_render_logic_probe_ch(uint32_t voltage, _Bool channel);

#endif

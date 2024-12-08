#ifndef ANSI_DISPLAY_H
#define ANSI_DISPLAY_H

#include <stdint.h>
#include "adc_control.h"

#define BORDER_VERTICAL "|"
#define BORDER_HORIZONTAL '-'

void ansi_set_background(const char* color);

void ansi_print_title(const unsigned short int ascii_type,
                      const char* color,
                      const char* bg_color);

void ansi_print_border(const char horizontal,
                       const char* vertical,
                       const char* color,
                       const char* bg_color);

void ansi_print_button(const char* text,
                       const char* color_text,
                       const char* bg_color,
                       const unsigned int row,
                       const unsigned int col);

void ansi_print_voltage_measures(const uint32_t v_ref,
                                 const uint32_t* v_measures,
                                 adc_channels* adc_ch);
void ansi_print_logic_probe_ch(uint32_t voltage, bool channel);
#endif  // ANSI_DISPLAY_H

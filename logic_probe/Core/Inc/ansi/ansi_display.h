#ifndef ANSI_DISPLAY_H
#define ANSI_DISPLAY_H

#include <stdint.h>
#include "adc_control.h"
#include "ansi_abstraction_layer.h"
#include "signal_detector.h"

#define BORDER_VERTICAL "|"
#define BORDER_HORIZONTAL '-'
#define COLORFUL_TEXT_T_SIZE 50
typedef struct {
    char str[COLORFUL_TEXT_T_SIZE];
    char color[COLOR_STR_SIZE];

} colorful_text_t;

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

void ansi_generate_frequency_reader(const sig_detector_t* detector);

void ansi_frequency_reader_generate_hint(void);
#endif  // ANSI_DISPLAY_H

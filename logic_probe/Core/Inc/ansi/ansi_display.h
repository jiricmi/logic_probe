#ifndef ANSI_DISPLAY_H
#define ANSI_DISPLAY_H

#include <stdint.h>
#include <stdio.h>
#define BORDER_VERTICAL "|"
#define BORDER_HORIZONTAL '-'

void ansi_set_background(const char* color);

void ansi_print_logo(const char* color, const char* bg_color);

void ansi_print_title(const char** ascii,
                      const size_t row,
                      const size_t col,
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
                                 const uint32_t ch1,
                                 const uint32_t ch2,
                                 const uint32_t ch3,
                                 const uint32_t ch4);
#endif  // ANSI_DISPLAY_H

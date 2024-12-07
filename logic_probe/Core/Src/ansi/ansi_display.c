#include "ansi_display.h"
#include <string.h>

#include "adc_control.h"
#include "ansi_abstraction_layer.h"
#include "ansi_ascii_text.h"
#include "utils.h"

void ansi_set_background(const char* color) {
    for (unsigned int i = 1; i <= TERMINAL_HEIGHT; ++i) {
        ansi_set_cursor(i, 1);
        send_uart_string(color);
        for (unsigned int j = 1; j <= TERMINAL_WIDTH; ++j) {
        }
    }

    ansi_clear_format();
}

void ansi_print_title(const unsigned short int ascii_type,
                      const char* color,
                      const char* bg_color) {
    ascii_text* ascii = get_ascii_struct(ascii_type);

    size_t start_display_row = 2;
    size_t width_center = TERMINAL_WIDTH / 2 - ((ascii->col - 1) / 2);

    for (size_t i = 0; i < ascii->row; ++i) {
        ansi_set_cursor(start_display_row + i, width_center);
        ansi_send_text(ascii->ascii[i], color, bg_color, 0);
    }
    free_ascii_text(ascii);
    ansi_clear_format();
}

void ansi_print_border(const char horizontal,
                       const char* vertical,
                       const char* color,
                       const char* bg_color) {
    char buffer[TERMINAL_WIDTH];

    memset(buffer, horizontal, sizeof(buffer));
    buffer[TERMINAL_WIDTH - 1] = '\0';

    ansi_set_cursor(0, 0);
    ansi_send_text(buffer, color, bg_color, 0);
    ansi_set_cursor(TERMINAL_HEIGHT - 1, 0);
    ansi_send_text(buffer, color, bg_color, 0);

    for (unsigned int i = 2; i < TERMINAL_HEIGHT - 1; ++i) {
        ansi_set_cursor(i, 0);
        ansi_send_text(vertical, color, bg_color, 0);
        ansi_set_cursor(i, TERMINAL_WIDTH - 1);
        ansi_send_text(vertical, color, bg_color, 0);
    }

    ansi_clear_format();
}

void ansi_print_button(const char* text,
                       const char* color_text,
                       const char* bg_color,
                       const unsigned int row,
                       const unsigned int col) {
    ansi_set_cursor(row, col);
    ansi_send_text("•", color_text, bg_color, 1);

    ansi_set_cursor(row, col + 2);
    ansi_send_text(text, color_text, bg_color, 1);
    ansi_clear_format();
}

void ansi_print_voltage_measures(const uint32_t v_ref,
                                 const uint32_t* measures,
                                 adc_channels* adc_ch) {
    char text_buffer[50];
    unsigned int split_float_format[2];
    static unsigned int floating_point = 3;
    unsigned int col_center = TERMINAL_WIDTH / 2 - 10;
    unsigned int row = 13;
    uint32_t avg_measures[4];
    adc_make_avg(avg_measures, adc_ch, measures);

    for (unsigned short int i = 0; i < NUM_CHANNELS; ++i) {
        ansi_set_cursor(row++, col_center);

        uint_32_to_split_int(split_float_format, get_voltage(avg_measures[i]),
                             floating_point);

        if (adc_ch->channel[i]) {
            snprintf(text_buffer, sizeof(text_buffer), "Channel %hu: %u.%0*u V",
                     i + 1, split_float_format[0], floating_point,
                     split_float_format[1]);
        } else {
            snprintf(text_buffer, sizeof(text_buffer), "Channel %hu: x", i + 1);
        }

        ansi_send_text(text_buffer, "", "", 1);
    }

    ansi_set_cursor(TERMINAL_HEIGHT - 5, col_center);
    uint_32_to_split_int(split_float_format, v_ref, floating_point);
    snprintf(text_buffer, sizeof(text_buffer), "Reference: %u.%u V",
             split_float_format[0], split_float_format[1]);
    ansi_send_text(text_buffer, "", "", 0);
    ansi_home_cursor();
}

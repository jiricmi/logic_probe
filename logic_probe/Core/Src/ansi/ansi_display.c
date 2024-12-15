#include "ansi_display.h"
#include <string.h>

#include "adc_control.h"
#include "ansi_abstraction_layer.h"
#include "ansi_ascii_text.h"
#include "signal_detector.h"
#include "stdio.h"
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

void print_voltage_for_channel(uint32_t voltage,
                               unsigned int channel,
                               unsigned int pin,
                               unsigned int floating_point) {
    char text_buffer[50];
    unsigned int split_float_format[2];

    uint_32_to_split_int(split_float_format, voltage, floating_point);
    snprintf(text_buffer, sizeof(text_buffer), "Channel %hu (A%u): %u.%0*u V",
             channel + 1, pin, split_float_format[0], floating_point,
             split_float_format[1]);
    ansi_send_text(text_buffer, "", "", 1);
}

void print_reference_voltage(uint32_t v_ref, unsigned int floating_point) {
    unsigned int split_float_format[2];
    uint_32_to_split_int(split_float_format, v_ref, floating_point);
    char text_buffer[50];
    snprintf(text_buffer, sizeof(text_buffer), "Reference: %u.%u V",
             split_float_format[0], split_float_format[1]);
    ansi_send_text(text_buffer, "", "", 0);
}

void ansi_print_voltage_measures(const uint32_t v_ref,
                                 const uint32_t* measures,
                                 adc_channels* adc_ch) {
    static const unsigned int floating_point = 3;
    const unsigned int col_center = TERMINAL_WIDTH / 2 - 13;
    unsigned int row = 13;
    uint32_t avg_measures[4];

    adc_make_avg(avg_measures, adc_ch, measures);

    for (unsigned short int channel = 0; channel < NUM_CHANNELS; ++channel) {
        uint32_t voltage = get_voltage(avg_measures[channel]);

        ansi_set_cursor(row, col_center + 24);
        ansi_print_logic_probe_ch(voltage, adc_ch->channel[channel]);

        ansi_set_cursor(row++, col_center);
        if (adc_ch->channel[channel]) {
            print_voltage_for_channel(voltage, channel, adc_ch->pin[channel],
                                      floating_point);
        } else {
            char text_buffer[50];
            snprintf(text_buffer, sizeof(text_buffer), "Channel %hu (A%u): x",
                     channel + 1, adc_ch->pin[channel]);
            ansi_send_text(text_buffer, "", "", 1);
        }
    }

    ansi_set_cursor(TERMINAL_HEIGHT - 5, col_center + 3);
    print_reference_voltage(v_ref, floating_point);

    ansi_home_cursor();
}

void ansi_print_logic_probe_ch(uint32_t voltage, bool channel) {
    if (!channel) {
        ansi_send_text(" ? ", WHITE_TEXT, LIGHT_GRAY_BG, 1);
        return;
    }

    if (voltage <= 800) {
        ansi_send_text(" L ", BLACK_TEXT, RED_BG, 1);
    } else if (voltage >= 2000) {
        ansi_send_text(" H ", BLACK_TEXT, GREEN_BG, 1);
    } else {
        ansi_send_text(" ? ", WHITE_TEXT, LIGHT_GRAY_BG, 1);
    }
}

void ansi_get_detector_mode(colorful_text_t* text, const uint8_t mode) {
    size_t buff_size = COLORFUL_TEXT_T_SIZE;
    switch (mode) {
        case 2:
            snprintf(text->str, buff_size, "%s", "Frequency/PWM");
            snprintf(text->color, COLOR_STR_SIZE, "%s", GREEN_TEXT);
            break;
        case 0:
            snprintf(text->str, buff_size, "%s", "Pulse Up");
            snprintf(text->color, COLOR_STR_SIZE, "%s", BLUE_TEXT);
            break;
        case 1:
            snprintf(text->str, buff_size, "%s", "Pulse Down");
            snprintf(text->color, COLOR_STR_SIZE, "%s", MAGENTA_TEXT);
            break;

        default:
            snprintf(text->str, buff_size, "%s", "Undefined");
            snprintf(text->color, COLOR_STR_SIZE, "%s", RED_TEXT);
    }
}

void ansi_generate_frequency_reader(const sig_detector_t* detector) {
    colorful_text_t col_text = {0};
    ansi_get_detector_mode(&col_text, detector->mode);
    ansi_set_cursor(8, 35);
    ansi_send_text(col_text.str, col_text.color, "", 1);

    uint32_t duty =
        (uint32_t)((float)detector->high_width /
                   (detector->high_width + detector->low_width) * 100);

    char buff[100];
    if (detector->mode == 2) {
        ansi_set_cursor(10, 10);
        snprintf(buff, 100, "Frequency (A0): %7lu Hz", detector->frequency);
        ansi_send_text(buff, "", "", 1);
        ansi_set_cursor(11, 10);
        snprintf(buff, 100, "High pulse width: %7lu us ", detector->high_width);
        ansi_send_text(buff, "", "", 1);
        ansi_set_cursor(12, 10);
        snprintf(buff, 100, "low pulse width: %7lu us ", detector->low_width);
        ansi_send_text(buff, "", "", 1);
        ansi_set_cursor(13, 10);
        snprintf(buff, 100, "Duty: %3lu %%", duty);
        ansi_send_text(buff, "", "", 1);
        ansi_set_cursor(14, 10);
        snprintf(buff, 100, "Sample time: %4u ms",
                 detector->sample_times[detector->sample_time_index]);
        ansi_send_text(buff, "", "", 1);
    } else if (detector->mode == 0) {
        ansi_set_cursor(10, 10);
        snprintf(buff, 100, "Pulse found: ");
        ansi_send_text(buff, "", "", 1);

        if (detector->p) {
            ansi_set_cursor(10, 14);
            snprintf(buff, 100, " TRUE ");
            ansi_send_text(buff, WHITE_TEXT, GREEN_BG, 1);
        }
    }
}

void ansi_frequency_reader_generate_hint(void) {
    ansi_set_cursor(TERMINAL_HEIGHT - 2, 4);
    ansi_send_text("m - change mode ", "", "", 0);
    ansi_set_cursor(TERMINAL_HEIGHT - 2, 21);
    ansi_send_text("t - change sample time ", "", "", 0);
}

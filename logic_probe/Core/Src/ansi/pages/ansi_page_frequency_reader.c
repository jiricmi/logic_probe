#include "ansi_page_frequency_reader.h"
#include <string.h>

#include "ansi_abstraction_layer.h"
#include "ansi_ascii_text.h"
#include "ansi_pages.h"
#include "global_vars.h"
#include "utils.h"

#define DETECTOR_MODE_BUFF_SIZE 50

extern global_vars_t global_var;

void ansi_render_frequency_reader_page(void) {
    global_var.current_page = ANSI_PAGE_FREQUENCY_READER;
    ansi_render_border('x', "x", "");
    ansi_render_title(ASCII_LOGO_SIGNAL_DETECT, YELLOW_TEXT);
    ansi_generate_frequency_reader(global_var.signal_detector);
    ansi_help_reader();
}

void ansi_generate_frequency_reader(sig_detector_t* detector) {
    uint8_t row = FREQ_READER_ROW_TEXT;
    uint8_t frequency_pin =
        (global_var.can_advanced) ? FREQUENCY_PIN_20 : FREQUENCY_PIN_8;
    ansi_get_detector_mode(detector->mode);
    char buff[BASE_TEXT_BUFF_LEN];
    if (detector->mode == DETECTOR_MODE_FREQUENCY) {
        char number_buff[NUM_BUFF_LEN];
        format_number_with_spaces(detector->frequency, number_buff);
        ansi_set_cursor(row++, FREQ_READER_COL_TEXT);
        snprintf(buff, BASE_TEXT_BUFF_LEN, "Frequency (PA0/A0): %12.12s Hz ",
                 number_buff);
        ansi_send_text(buff, &ansi_default_conf);

        format_number_with_spaces(detector->rec_frequency, number_buff);
        ansi_set_cursor(row++, FREQ_READER_COL_TEXT);
        snprintf(buff, BASE_TEXT_BUFF_LEN, "Reciprocial frequency: %12.12s Hz",
                 number_buff);
        ansi_send_text(buff, &ansi_default_conf);

        ansi_set_cursor(row++, FREQ_READER_COL_TEXT);
        format_number_with_spaces(detector->widths[DET_HIGH_WIDTH],
                                  number_buff);
        snprintf(buff, BASE_TEXT_BUFF_LEN, "High pulse width: %9.9s us ",
                 number_buff);
        ansi_send_text(buff, &ansi_default_conf);

        ansi_set_cursor(row++, FREQ_READER_COL_TEXT);
        format_number_with_spaces(detector->widths[DET_LOW_WIDTH], number_buff);
        snprintf(buff, BASE_TEXT_BUFF_LEN, "Low pulse width: %9.9s us ",
                 number_buff);
        ansi_send_text(buff, &ansi_default_conf);

        ansi_set_cursor(row++, FREQ_READER_COL_TEXT);
        snprintf(buff, BASE_TEXT_BUFF_LEN, "Duty: %3u %%",
                 (uint16_t)detector->pwm_duty);
        ansi_send_text(buff, &ansi_default_conf);

        ansi_set_cursor(row++, FREQ_READER_COL_TEXT);
        snprintf(buff, BASE_TEXT_BUFF_LEN, "Gate time: %5u ms",
                 GATE_TIMES[detector->gate_time_index]);
        ansi_send_text(buff, &ansi_default_conf);

    } else if (detector->mode != DETECTOR_MODE_FREQUENCY) {
        char pulse_text[] = "Pulse found (PA0): ";
        ansi_set_cursor(FREQ_READER_ROW_TEXT,
                        TERMINAL_CENTER - strlen(pulse_text) / 2);
        ansi_send_text(pulse_text, &ansi_default_conf);
        ansi_text_config_t text_conf = {"", RED_BG, 1};

        ansi_set_cursor(FREQ_READER_ROW_TEXT,
                        TERMINAL_CENTER + strlen(pulse_text) / 2);
        if (detector->one_pulse_found) {
            text_conf.color = WHITE_TEXT;
            text_conf.bg_color = GREEN_BG;
            ansi_send_text(" TRUE  ", &text_conf);
        } else {
            ansi_send_text(" FALSE ", &text_conf);
        }
    }
}

void ansi_get_detector_mode(detector_mode_t mode) {
    ansi_set_cursor(FREQ_READER_ROW_MODE, TERMINAL_CENTER - 8);
    ansi_text_config_t text_conf = {WHITE_TEXT, "", 1};

    switch (mode) {
        case DETECTOR_MODE_FREQUENCY:
            ansi_send_text("Frequency/PWM", &text_conf);
            break;
        case DETECTOR_MODE_PULSE_UP:
            text_conf.color = BLUE_TEXT;
            ansi_send_text("Rise Edge Pulse", &text_conf);
            break;
        case DETECTOR_MODE_PULSE_DOWN:
            text_conf.color = MAGENTA_TEXT;
            ansi_send_text("Fall Edge Pulse", &text_conf);
            break;

        default:
            text_conf.color = RED_TEXT;
            ansi_send_text("Undefined", &text_conf);
    }
}

void ansi_help_reader(void) {
    if (global_var.signal_detector->mode == DETECTOR_MODE_FREQUENCY) {
        ansi_print_help_msg("T: change gate time | M: one pulse mode", 0);
    } else {
        ansi_print_help_msg("D: delete catch flag | M: change mode", 0);
    }
}

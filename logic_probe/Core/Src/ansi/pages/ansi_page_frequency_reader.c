#include "ansi_page_frequency_reader.h"

#include "ansi_abstraction_layer.h"
#include "ansi_ascii_text.h"
#include "ansi_pages.h"
#include "global_vars.h"

#define DETECTOR_MODE_BUFF_SIZE 50

extern global_vars_t global_var;

void ansi_render_frequency_reader_page(void) {
    global_var.current_page = ANSI_PAGE_FREQUENCY_READER;
    ansi_render_border('x', "x", "");
    ansi_render_title(ASCII_LOGO_SIGNAL_DETECT, YELLOW_TEXT);
    ansi_generate_frequency_reader(global_var.signal_detector);
    ansi_frequency_reader_generate_hint();
}

void ansi_generate_frequency_reader(sig_detector_t* detector) {
    char buffer[DETECTOR_MODE_BUFF_SIZE];
    ansi_text_config_t conf = {"", "", 1};
    ansi_get_detector_mode(buffer, &conf, detector->mode);
    ansi_set_cursor(8, 35);
    ansi_send_text(buffer, &conf);

    uint32_t duty = 0;
    if (detector->low_width != 0 || detector->high_width != 0) {
        duty = (uint32_t)((float)detector->high_width /
                          (detector->high_width + detector->low_width) * 100);
    }

    char buff[100];
    if (detector->mode == 2) {
        ansi_set_cursor(10, 10);
        snprintf(buff, 100, "Frequency (A0): %7lu Hz", detector->frequency);
        ansi_send_text(buff, &ansi_default_conf);
        ansi_set_cursor(11, 10);
        snprintf(buff, 100, "High pulse width: %7lu us ", detector->high_width);
        ansi_send_text(buff, &ansi_default_conf);
        ansi_set_cursor(12, 10);
        snprintf(buff, 100, "low pulse width: %7lu us ", detector->low_width);
        ansi_send_text(buff, &ansi_default_conf);
        ansi_set_cursor(13, 10);
        snprintf(buff, 100, "Duty: %3lu %%", duty);
        ansi_send_text(buff, &ansi_default_conf);
        ansi_set_cursor(14, 10);
        snprintf(buff, 100, "Sample time: %4u ms",
                 detector->sample_times[detector->sample_time_index]);
        ansi_send_text(buff, &ansi_default_conf);
    } else if (detector->mode != 2) {
        ansi_set_cursor(10, 10);
        snprintf(buff, 100, "Pulse found: ");
        ansi_send_text(buff, &ansi_default_conf);
        ansi_text_config_t text_conf = {"", RED_BG, 1};

        if (detector->p) {
            ansi_set_cursor(10, 24);
            snprintf(buff, 100, " TRUE ");
            text_conf.color = WHITE_TEXT;
            text_conf.bg_color = GREEN_BG;
            ansi_send_text(buff, &text_conf);
        } else {
            ansi_set_cursor(10, 24);
            snprintf(buff, 100, " FALSE ");
            ansi_send_text(buff, &text_conf);
        }
    }
}

void ansi_get_detector_mode(char* text,
                            ansi_text_config_t* text_conf,
                            detector_mode_t mode) {
    size_t buff_size = DETECTOR_MODE_BUFF_SIZE;
    switch (mode) {
        case DETECTOR_MODE_FREQUENCY:
            snprintf(text, buff_size, "%s", "Frequency/PWM");
            snprintf(text_conf->color, COLOR_STR_SIZE, "%s", GREEN_TEXT);
            break;
        case DETECTOR_MODE_PULSE_UP:
            snprintf(text, buff_size, "%s", "Pulse Up");
            snprintf(text_conf->color, COLOR_STR_SIZE, "%s", BLUE_TEXT);
            break;
        case DETECTOR_MODE_PULSE_DOWN:
            snprintf(text, buff_size, "%s", "Pulse Down");
            snprintf(text_conf->color, COLOR_STR_SIZE, "%s", MAGENTA_TEXT);
            break;

        default:
            snprintf(text, buff_size, "%s", "Undefined");
            snprintf(text_conf->color, COLOR_STR_SIZE, "%s", RED_TEXT);
    }
}

void ansi_frequency_reader_generate_hint(void) {
    ansi_text_config_t conf = {"", "", 0};
    ansi_set_cursor(TERMINAL_HEIGHT - 2, 4);
    ansi_send_text("m - change mode ", &conf);
    ansi_set_cursor(TERMINAL_HEIGHT - 2, 21);
    ansi_send_text("t - change sample time ", &conf);
    ansi_set_cursor(TERMINAL_HEIGHT - 2, 45);
    ansi_send_text("d - delete flag ", &conf);
}

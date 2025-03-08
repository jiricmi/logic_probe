#include "ansi_page_voltage_measure.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "adc_control.h"
#include "global_vars.h"
#include "utils.h"

#include "ansi_abstraction_layer.h"
#include "ansi_ascii_text.h"
#include "ansi_pages.h"

extern global_vars_t global_var;

void ansi_render_voltage_page(void) {
    global_var.current_page = ANSI_PAGE_VOLTAGE_MEASURE;
    ansi_render_border('@', "@", "");
    ansi_render_title(ASCII_LOGO_VOLTAGE, MAGENTA_TEXT);
    ansi_render_voltage_measures(global_var.adc_vars);
}

void ansi_render_voltage_measures(const adc_vars_t* adc_ch) {
    const uint8_t col_center = TERMINAL_WIDTH / 2 - 9;
    uint8_t row = 13;
    uint32_t floating_avg_measures[NUM_CHANNELS];

    adc_calculate_floating_voltage_avg(floating_avg_measures, adc_ch);

    uint32_t ref_voltage = adc_get_v_ref(floating_avg_measures[0]);

    for (uint8_t channel = 1; channel < NUM_CHANNELS; ++channel) {
        uint32_t channel_voltage =
            adc_get_voltage(ref_voltage, floating_avg_measures[channel]);

        ansi_set_cursor(row, col_center + 24);
        ansi_render_logic_probe_ch(channel_voltage,
                                   adc_ch->channel_state[channel]);

        ansi_render_state_channel(row, col_center - 4, channel, adc_ch);

        ansi_set_cursor(row++, col_center);
        if (adc_ch->channel_state[channel]) {
            ansi_render_channel_voltage(channel_voltage, channel,
                                        adc_ch->pin[channel],
                                        ADC_FLOATING_POINT);
        } else {
            char text_buffer[ANSI_VOLTAGE_TEXT_BUFFER];
            snprintf(text_buffer, sizeof(text_buffer), "Channel %hu (A%u): x",
                     channel, (unsigned int)adc_ch->pin[channel]);
            ansi_send_text(text_buffer, &ansi_bold_conf);
        }
    }

    ansi_set_cursor(TERMINAL_HEIGHT - 6, col_center);
    ansi_render_reference_voltage(ref_voltage, floating_point);
    ansi_render_adc_change_message(TERMINAL_HEIGHT - 4, col_center,
                                   adc_ch);  // TODO: FIX

    ansi_home_cursor();
}

void ansi_render_state_channel(const uint8_t row,
                               const uint8_t col,
                               const uint8_t channel,
                               const adc_vars_t* adc_ch) {
    _Bool channel_state = adc_ch->channel_state[channel];
    _Bool channel_state_unapplied = adc_ch->channel_state_unapplied[channel];

    ansi_text_config_t channel_state_conf = {"", "", true};

    if (channel_state && channel_state_unapplied) {
    } else if (!channel_state && !channel_state_unapplied) {
        channel_state_conf.bg_color = RED_BG;

    } else if (channel_state && !channel_state_unapplied) {
        channel_state_conf.bg_color = DARK_YELLOW_BG;
    } else if (channel_state_unapplied) {
        channel_state_conf.bg_color = GREEN_BG;
    }

    char text_buffer[ANSI_VOLTAGE_TEXT_BUFFER];
    ansi_set_cursor(row, col);
    snprintf(text_buffer, sizeof(text_buffer), "[%hu]", channel);

    ansi_send_text(text_buffer, &channel_state_conf);
}

void ansi_render_adc_change_message(const uint8_t row,
                                    const uint8_t col,
                                    const adc_vars_t* adc_ch) {
    char buff[] = "Change was made! Press S to save settings.";
    uint8_t buff_len = (uint8_t)strlen(buff) / 2;

    uint8_t center = 0;
    if (col - buff_len > 0) {
        center = col - buff_len;
    }

    if (!adc_ch->applied) {
        ansi_set_cursor(row, center);
        ansi_text_config_t text_conf = {RED_TEXT, "", 0};
        ansi_send_text(buff, &text_conf);
    } else {
        ansi_clear_line(row, col);
    }
}

void ansi_render_channel_voltage(uint32_t voltage,
                                 unsigned int channel,
                                 unsigned int pin,
                                 unsigned int floating_point) {
    char text_buffer[ANSI_VOLTAGE_TEXT_BUFFER];
    unsigned int split_float_format[2];

    uint_32_to_split_int(split_float_format, voltage, floating_point);
    snprintf(text_buffer, sizeof(text_buffer), "Channel %hu (A%u): %u.%0*u V",
             channel, pin, split_float_format[0], floating_point,
             split_float_format[1]);
    ansi_send_text(text_buffer, &ansi_bold_conf);
}

void ansi_render_reference_voltage(uint32_t v_ref,
                                   unsigned int floating_point) {
    unsigned int split_float_format[2];
    uint_32_to_split_int(split_float_format, v_ref, floating_point);
    char text_buffer[ANSI_VOLTAGE_TEXT_BUFFER];
    snprintf(text_buffer, sizeof(text_buffer), "Reference: %u.%u V",
             split_float_format[0], split_float_format[1]);
    ansi_send_text(text_buffer, &ansi_default_conf);
}

void ansi_render_logic_probe_ch(uint32_t voltage, _Bool channel) {
    ansi_text_config_t conf_unknown_state = {WHITE_TEXT, LIGHT_GRAY_BG, 1};
    if (!channel) {
        ansi_send_text(" ? ", &conf_unknown_state);
        return;
    }
    ansi_text_config_t v_text_conf = {BLACK_TEXT, RED_BG, 1};

    if (voltage <= LOW_MAX_V) {
        ansi_send_text(" L ", &v_text_conf);
    } else if (voltage >= HIGH_MIN_V) {
        v_text_conf.bg_color = GREEN_BG;
        ansi_send_text(" H ", &v_text_conf);
    } else {
        ansi_send_text(" ? ", &conf_unknown_state);
    }
}

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

_Bool ansi_page_voltage_edit_resistance = false;

void ansi_render_voltage_page(void) {
    global_var.current_page = ANSI_PAGE_VOLTAGE_MEASURE;
    ansi_render_border('@', "@", "");
    if (global_var.adc_vars->resistance_mode) {
        ansi_render_resistance_circuit(8, ADC_MEASURE_CENTER);
        ansi_render_resistance_measure(global_var.adc_vars);
    } else {
        ansi_render_title(ASCII_LOGO_VOLTAGE, MAGENTA_TEXT);
        ansi_render_voltage_measures(global_var.adc_vars);
    }
}

void ansi_render_resistance_measure(const adc_vars_t* adc_ch) {
    uint32_t floating_avg_measures[ADC_NUM_CHANNELS];

    adc_calculate_floating_voltage_avg(floating_avg_measures, adc_ch);

    uint32_t ref_voltage = adc_get_v_ref(floating_avg_measures[0]);
    uint32_t measured_voltage =
        adc_get_voltage(ref_voltage, floating_avg_measures[1]);

    uint32_t resistance = (adc_ch->base_resistor * measured_voltage) /
                          (ref_voltage - measured_voltage);

    uint8_t row = 8;

    ansi_set_cursor(row - 3, ADC_MEASURE_CENTER + 3);
    ansi_send_text("RESISTANCE", &ansi_bold_conf);

    ansi_render_resistance_values(row, ADC_MEASURE_CENTER, ref_voltage,
                                  adc_ch->base_resistor, resistance);

    if (ansi_page_voltage_edit_resistance) {
        ansi_set_cursor(TERMINAL_HEIGHT - 3, ADC_MEASURE_CENTER);
        ansi_send_text("Editing base resistor!", &ansi_red_bold_conf);
    }
}

void ansi_render_resistance_circuit(const uint8_t row, const uint8_t col) {
    // clang-format off
    const char* circuit[] = {
        "    Vcc",
        "     |",
        "     |",
        " [R1 BASE]",
        "     |",
        "     +--- CHANNEL 1",
        "     |",
        "    [R2]",
        "     |",
        "     |",
        "    GND"
    };
    // clang-format on
    uint8_t lines = sizeof(circuit) / sizeof(circuit[0]);

    for (uint8_t i = 0; i < lines; ++i) {
        ansi_set_cursor(row + i, col);
        ansi_send_text(circuit[i], &ansi_default_conf);
    }
}

void ansi_render_resistance_values(const uint8_t row,
                                   const uint8_t col,
                                   const uint32_t volt_ref,
                                   const uint32_t base_resistor,
                                   const uint32_t resistance) {
    char text_buffer[ANSI_VOLTAGE_TEXT_BUFFER];

    ansi_set_cursor(row, col + VOLTAGE_TEXT_OFFSET);
    snprintf(text_buffer, sizeof(text_buffer), " (%4lu mV)", volt_ref);
    ansi_send_text(text_buffer, &ansi_bold_conf);

    ansi_set_cursor(row + 3, col + BASE_RESISTOR_OFFSET);
    snprintf(text_buffer, sizeof(text_buffer), "\u03A9 %7lu", base_resistor);
    ansi_send_text(text_buffer, &ansi_bold_conf);

    ansi_text_config_t result_text_conf = {"", GREEN_BG, true};
    ansi_set_cursor(row + 7, col + RESISTANCE_OFFSET);
    snprintf(text_buffer, sizeof(text_buffer), "\u03A9 %7lu ", resistance);
    ansi_send_text(text_buffer, &result_text_conf);
}

void ansi_render_voltage_measures(const adc_vars_t* adc_ch) {
    const uint8_t col_center = ADC_MEASURE_CENTER;
    uint8_t row = ADC_MEASURE_ROW;
    uint32_t floating_avg_measures[ADC_NUM_CHANNELS];

    adc_calculate_floating_voltage_avg(floating_avg_measures, adc_ch);

    uint32_t ref_voltage = adc_get_v_ref(floating_avg_measures[0]);

    for (uint8_t channel = 1; channel < ADC_NUM_CHANNELS; ++channel) {
        uint32_t channel_voltage =
            adc_get_voltage(ref_voltage, floating_avg_measures[channel]);

        ansi_set_cursor(row, col_center + 28);
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
            snprintf(text_buffer, sizeof(text_buffer),
                     "Channel %hu (Pin %lu): x", channel, adc_ch->pin[channel]);
            ansi_send_text(text_buffer, &ansi_bold_conf);
        }
    }

    ansi_set_cursor(TERMINAL_HEIGHT - 6, col_center);
    ansi_render_reference_voltage(ref_voltage, ADC_FLOATING_POINT);
    ansi_render_adc_change_message(TERMINAL_HEIGHT - 4, col_center, adc_ch);

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
        ansi_send_text(buff, &ansi_red_bold_conf);
    } else {
        ansi_clear_line(row, col);
    }
}

void ansi_render_channel_voltage(uint32_t voltage,
                                 uint8_t channel,
                                 uint32_t pin,
                                 uint32_t floating_point) {
    char text_buffer[ANSI_VOLTAGE_TEXT_BUFFER];
    uint32_t split_float_format[2];

    uint_32_to_split_int(split_float_format, voltage, floating_point);
    snprintf(text_buffer, sizeof(text_buffer),
             "Channel %hu (Pin %lu): %lu.%0*lu V", channel, pin,
             split_float_format[0], (int)floating_point, split_float_format[1]);
    ansi_send_text(text_buffer, &ansi_bold_conf);
}

void ansi_render_reference_voltage(uint32_t v_ref, uint32_t floating_point) {
    uint32_t split_float_format[2];
    uint_32_to_split_int(split_float_format, v_ref, floating_point);
    char text_buffer[ANSI_VOLTAGE_TEXT_BUFFER];
    snprintf(text_buffer, sizeof(text_buffer), "Reference: %lu.%lu V",
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

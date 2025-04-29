#include "ansi_page_voltage_measure.h"

#include <stdbool.h>
#include <stdint.h>
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
    if (global_var.device_state == DEV_STATE_VOLTMETER) {
        ansi_render_title(ASCII_LOGO_VOLTAGE, MAGENTA_TEXT);
        ansi_render_voltage_measures(&global_var.adc_perif);
        help_voltage();
    } else {
        ansi_render_resistance_circuit(8, ADC_MEASURE_CENTER);
        ansi_render_resistance_measure(&global_var.adc_perif);
        help_resistance();
    }
}

void ansi_render_resistance_circuit(const uint8_t row, const uint8_t col) {
    // clang-format off
    const char* circuit[] = {
        "    Vdd",
        "     |",
        "     |",
        " [R1 BASE]",
        "     |",
        "     +--- GPIO 26",
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

void ansi_render_resistance_measure(const adc_vars_t* adc_ch) {
    uint32_t measured_voltage = adc_get_voltage(adc_ch->avg_voltage[0]);

    uint32_t resistance =
        (adc_ch->base_resistor * measured_voltage) / (3300 - measured_voltage);

    uint8_t row = 8;

    ansi_set_cursor(row - 3, ADC_MEASURE_CENTER + 3);
    ansi_send_text("RESISTANCE (GPIO 26)", &ansi_bold_conf);

    ansi_render_resistance_values(row, ADC_MEASURE_CENTER,
                                  adc_ch->base_resistor, resistance);

    if (global_var.adc_perif.edit_resistance) {
        ansi_set_cursor(TERMINAL_HEIGHT - 3, ADC_MEASURE_CENTER);
        ansi_send_text("Editing base resistor!", &ansi_red_bold_conf);
    }
}

void ansi_render_resistance_values(const uint8_t row,
                                   const uint8_t col,
                                   const uint32_t base_resistor,
                                   const uint32_t resistance) {
    char text_buffer[ANSI_VOLTAGE_TEXT_BUFFER];
    char number_buffer[20];

    ansi_set_cursor(row, col + VOLTAGE_TEXT_OFFSET);
    ansi_send_text(" (3 300 mV)", &ansi_bold_conf);

    ansi_set_cursor(row + 3, col + BASE_RESISTOR_OFFSET);

    format_number_with_spaces(base_resistor, number_buffer);
    snprintf(text_buffer, sizeof(text_buffer), "\u03A9 %10.10s", number_buffer);
    ansi_send_text(text_buffer, &ansi_bold_conf);

    ansi_text_config_t result_text_conf = {"", GREEN_BG, true};
    ansi_set_cursor(row + 7, col + RESISTANCE_OFFSET);
    format_number_with_spaces(resistance, number_buffer);
    snprintf(text_buffer, sizeof(text_buffer), "\u03A9 %13.13s ",
             number_buffer);
    ansi_send_text(text_buffer, &result_text_conf);
}

void ansi_render_voltage_measures(const adc_vars_t* adc_ch) {
    const uint8_t col_center = ADC_MEASURE_CENTER;
    uint8_t row = ADC_MEASURE_ROW;

    for (uint8_t channel = 0; channel < ADC_NUM_CHANNELS; ++channel) {
        uint32_t channel_voltage =
            adc_get_voltage(adc_ch->avg_voltage[channel]);

        ansi_set_cursor(row, col_center + 29);
        ansi_render_logic_probe_ch(channel_voltage);

        ansi_set_cursor(row++, col_center);
        ansi_render_channel_voltage(channel_voltage, channel,
                                    adc_ch->pinout[channel],
                                    ADC_FLOATING_POINT);
    }

    ansi_set_cursor(TERMINAL_HEIGHT - 6, col_center);

    ansi_home_cursor();
}

void ansi_render_channel_voltage(const uint32_t voltage,
                                 const uint8_t channel,
                                 const uint8_t pin_num,
                                 const uint32_t floating_point) {
    char text_buffer[ANSI_VOLTAGE_TEXT_BUFFER];
    uint32_t split_float_format[2];

    uint_32_to_split_int(split_float_format, voltage, floating_point);
    snprintf(text_buffer, sizeof(text_buffer),
             "Channel %hu (GPIO %d): %lu.%0*lu V", channel + 1, pin_num,
             split_float_format[0], (int)floating_point, split_float_format[1]);
    ansi_send_text(text_buffer, &ansi_bold_conf);
}

void ansi_render_logic_probe_ch(const uint32_t voltage) {
    ansi_text_config_t conf_unknown_state = {WHITE_TEXT, LIGHT_GRAY_BG, 1};
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

void help_voltage(void) {
    char msg[] = "M: resistance mode";
    ansi_print_help_msg(msg, 0);
}

void help_resistance(void) {
    if (global_var.adc_perif.edit_resistance) {
        ansi_print_help_msg(
            "0-9: enter number | x: delete number | e: exit edit mode", 0);
    } else {
        ansi_print_help_msg("e: edit base resistor | M: voltage mode", 0);
    }
}

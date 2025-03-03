#include "ansi_page_voltage_measure.h"

#include <stdint.h>
#include "adc_control.h"
#include "utils.h"

#include "ansi_abstraction_layer.h"
#include "ansi_ascii_text.h"
#include "ansi_pages.h"

extern uint32_t v_ref;
extern uint32_t* v_measures;
extern adc_channels* adc1_ch;
extern ansi_page_type_t current_page;

void ansi_render_voltage_page(void) {
    current_page = ANSI_PAGE_VOLTAGE_MEASURE;
    ansi_render_border('@', "@", "");
    ansi_render_title(ASCII_LOGO_VOLTAGE, MAGENTA_TEXT);
    ansi_render_voltage_measures(v_ref, v_measures, adc1_ch);
}

void ansi_render_voltage_measures(const uint32_t v_ref1,
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
        ansi_render_logic_probe_ch(voltage, adc_ch->channel[channel]);

        ansi_set_cursor(row++, col_center);
        if (adc_ch->channel[channel]) {
            ansi_render_channel_voltage(voltage, channel, adc_ch->pin[channel],
                                        floating_point);
        } else {
            char text_buffer[50];
            snprintf(text_buffer, sizeof(text_buffer), "Channel %hu (A%u): x",
                     channel + 1, adc_ch->pin[channel]);
            ansi_send_text(text_buffer, &ansi_bold_conf);
        }
    }

    ansi_set_cursor(TERMINAL_HEIGHT - 5, col_center + 3);
    ansi_render_reference_voltage(v_ref, floating_point);

    ansi_home_cursor();
}

void ansi_render_channel_voltage(uint32_t voltage,
                                 unsigned int channel,
                                 unsigned int pin,
                                 unsigned int floating_point) {
    char text_buffer[50];
    unsigned int split_float_format[2];

    uint_32_to_split_int(split_float_format, voltage, floating_point);
    snprintf(text_buffer, sizeof(text_buffer), "Channel %hu (A%u): %u.%0*u V",
             channel + 1, pin, split_float_format[0], floating_point,
             split_float_format[1]);
    ansi_send_text(text_buffer, &ansi_bold_conf);
}

void ansi_render_reference_voltage(uint32_t v_ref1,
                                   unsigned int floating_point) {
    unsigned int split_float_format[2];
    uint_32_to_split_int(split_float_format, v_ref, floating_point);
    char text_buffer[50];
    snprintf(text_buffer, sizeof(text_buffer), "Reference: %u.%u V",
             split_float_format[0], split_float_format[1]);
    ansi_send_text(text_buffer, &ansi_default_conf);
}

void ansi_render_logic_probe_ch(uint32_t voltage, bool channel) {
    ansi_text_config_t conf_unknown_state = {WHITE_TEXT, LIGHT_GRAY_BG, 1};
    if (!channel) {
        ansi_send_text(" ? ", &conf_unknown_state);
        return;
    }
    ansi_text_config_t v_text_conf = {BLACK_TEXT, RED_BG, 1};

    if (voltage <= 800) {
        ansi_send_text(" L ", &v_text_conf);
    } else if (voltage >= 2000) {
        v_text_conf.bg_color = GREEN_BG;
        ansi_send_text(" H ", &v_text_conf);
    } else {
        ansi_send_text(" ? ", &conf_unknown_state);
    }
}

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "advanced/neopixel.h"
#include "ansi_abstraction_layer.h"
#include "ansi_page_voltage_measure.h"
#include "ansi_pages_neopixel_measure.h"
#include "global_vars.h"

extern global_vars_t global_var;

void ansi_render_neopixel_measure_page(void) {
    global_var.current_page = ANSI_PAGE_NEOPIXEL_MEASURE;
    ansi_render_border('^', "^", "");
    if (global_var.device_state == DEV_STATE_ADV_NEOPIXEL_READ) {
        ansi_set_cursor(4, ADC_MEASURE_CENTER);
        ansi_send_text("NEOPIXEL READ (PIN PA0)", &ansi_bold_conf);
        ansi_render_neopixel_measure_vals(global_var.adv_neopixel_measure);
    } else if (global_var.device_state == DEV_STATE_ADV_NEOPIXEL_WRITE) {
        ansi_set_cursor(4, ADC_MEASURE_CENTER);
        ansi_send_text("NEOPIXEL SEND (PIN PA13)", &ansi_bold_conf);
        ansi_render_neopixel_send_vals(global_var.adv_neopixel_measure);
    }
}

void ansi_render_neopixel_measure_vals(neopixel_measure_t* data) {
    ansi_set_cursor(8, ADC_MEASURE_CENTER);
    neopixel_read_parse(global_var.adv_neopixel_measure);
    char buff[50];
    snprintf(buff, 50, "R: %3u G: %3u B: %3u", data->rgb[0], data->rgb[1],
             data->rgb[2]);

    ansi_send_text(buff, &ansi_default_conf);
}

void ansi_render_neopixel_send_vals(neopixel_measure_t* data) {
    ansi_set_cursor(8, ADC_MEASURE_CENTER);
    neopixel_read_parse(global_var.adv_neopixel_measure);
    const char rgb_letters[] = {'R', 'G', 'B'};

    char buff[50];
    for (uint8_t i = 0; i < 3; ++i) {
        snprintf(buff, 50, "%c: %3u ", rgb_letters[i], data->rgb_send[i]);
        if (data->edit && data->rgb_index == i) {
            ansi_send_text(buff, &ansi_green_bold_conf);
        } else {
            ansi_send_text(buff, &ansi_default_conf);
        }
    }

    snprintf(buff, 50, "Repeat: %2u x", data->rgb_send[3]);
    ansi_set_cursor(9, TERMINAL_CENTER - (strlen(buff) / 2));
    if (data->edit && data->rgb_index == 3) {
        ansi_send_text(buff, &ansi_green_bold_conf);
    } else {
        ansi_send_text(buff, &ansi_default_conf);
    }

    ansi_set_cursor(12, ADC_MEASURE_CENTER);
    if (data->edit) {
        ansi_send_text("Editing!", &ansi_red_bold_conf);
    }
}

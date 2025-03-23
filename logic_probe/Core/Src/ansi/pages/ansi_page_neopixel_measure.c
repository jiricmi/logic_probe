#include <stdio.h>
#include "advanced/neopixel.h"
#include "ansi_abstraction_layer.h"
#include "ansi_page_voltage_measure.h"
#include "ansi_pages_neopixel_measure.h"
#include "global_vars.h"

extern global_vars_t global_var;

void ansi_render_neopixel_measure_page(void) {
    global_var.current_page = ANSI_PAGE_NEOPIXEL_MEASURE;
    ansi_render_border('^', "^", "");
    ansi_render_neopixel_measure_vals(global_var.adv_neopixel_measure);
}

void ansi_render_neopixel_measure_vals(neopixel_measure_t* data) {
    ansi_set_cursor(8, ADC_MEASURE_CENTER);
    neopixel_read_parse(global_var.adv_neopixel_measure);
    char buff[50];
    snprintf(buff, 50, "r: %3u g: %3u b: %3u", data->rgb[0], data->rgb[1],
             data->rgb[2]);

    ansi_send_text(buff, &ansi_default_conf);
}

#include "ansi_page_shift_register.h"
#include "advanced/shift_register.h"
#include "ansi_abstraction_layer.h"
#include "ansi_page_voltage_measure.h"
#include "ansi_pages.h"
#include "global_vars.h"

extern global_vars_t global_var;

void ansi_render_shift_register_page(void) {
    ansi_render_border('*', "*", "");
    ansi_set_cursor(4, ADC_MEASURE_CENTER);
    ansi_send_text("SHIFT REGISTER", &ansi_bold_conf);
    ansi_set_cursor(5, ADC_MEASURE_CENTER - 4);
    ansi_send_text("PB7/1-CLOCK | PA0/7-DATA | PA1/8-RCLK", &ansi_bold_conf);
    ansi_set_cursor(7, ADC_MEASURE_CENTER - 10);
    char buff[REGISTER_BUFF_TEXT];
    for (uint8_t i = 0; i < SHIFT_REGISTER_BIT_COUNT; ++i) {
        snprintf(buff, REGISTER_BUFF_TEXT, "| %d |",
                 global_var.adv_shift_register->bits[i]);
        if (i == global_var.adv_shift_register->index) {
            ansi_send_text(buff, &ansi_green_bold_conf);
        } else {
            ansi_send_text(buff, &ansi_bold_conf);
        }
    }
    help_shift_register();
}

void help_shift_register(void) {
    ansi_print_help_msg(
        "1-8: Toggle bit | S: send | D: send one bit | F: reset counter | G: "
        "send RCLK",
        0);
}

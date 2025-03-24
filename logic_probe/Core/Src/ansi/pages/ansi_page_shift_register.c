#include "ansi_page_shift_register.h"
#include "advanced/shift_register.h"
#include "ansi_abstraction_layer.h"
#include "ansi_page_voltage_measure.h"
#include "global_vars.h"

extern global_vars_t global_var;

void ansi_render_shift_register_page(void) {
    ansi_render_border('*', "*", "");
    ansi_set_cursor(4, ADC_MEASURE_CENTER);
    ansi_send_text("SHIFT REGISTER", &ansi_bold_conf);
    ansi_set_cursor(7, ADC_MEASURE_CENTER - 10);
    char buff[50];
    for (uint8_t i = 0; i < SHIFT_REGISTER_BIT_COUNT; ++i) {
        snprintf(buff, 50, "| %d |", global_var.adv_shift_register->bits[i]);
        ansi_send_text(buff, &ansi_bold_conf);
    }
}

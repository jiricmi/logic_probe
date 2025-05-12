#include "ansi_page_impulse_generator.h"
#include <stdbool.h>
#include "ansi_abstraction_layer.h"
#include "ansi_ascii_text.h"

#include "ansi_pages.h"
#include "global_vars.h"
#include "main.h"
#include "utils.h"

extern global_vars_t global_var;

void ansi_render_impulse_generator_page(void) {
    global_var.current_page = ANSI_PAGE_IMPULSE_GENERATOR;
    ansi_render_border('%', "%", "");
    ansi_render_title(ASCII_LOGO_SIGNAL_GENERATOR, CYAN_TEXT);
    ansi_render_impulse_generator(global_var.signal_generator);
}

void ansi_render_impulse_generator(const sig_generator_t* generator) {
    char buff[50];
    char number_buff[15];
    ansi_set_cursor(9, 10);
    if (generator->mode == SIG_GEN_MODE_PULSE_UP) {
        ansi_send_text("UP  ", &ansi_default_conf);
    } else {
        ansi_send_text("DOWN", &ansi_default_conf);
    }
    ansi_set_cursor(10, 10);
    format_number_with_spaces(generator->period_us_temp, number_buff);
    snprintf(buff, 50, "Pulse width (PA0/" FREQUENCY_PIN "): %9.9s us",
             number_buff);
    ansi_send_text(buff, &ansi_default_conf);

    uint64_t freq = (generator->period_us_temp != 0)
                        ? 1000000 / (2 * generator->period_us_temp)
                        : 0;

    ansi_set_cursor(11, 10);
    format_number_with_spaces((uint32_t)freq, number_buff);
    snprintf(buff, 50, "Frequency in permament sending: %7.7s Hz",
             number_buff);
    ansi_send_text(buff, &ansi_default_conf);

    GPIO_PinState state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
    ansi_set_cursor(12, 10);
    if (state == GPIO_PIN_SET) {
        snprintf(buff, 50, "INPUT: HIGH");
    } else {
        snprintf(buff, 50, "INPUT: LOW ");
    }
    ansi_send_text(buff, &ansi_default_conf);
    ansi_set_cursor(13, 10);
    snprintf(buff, 50, "Repeats: %2ux", generator->repeat);
    ansi_send_text(buff, &ansi_default_conf);

    ansi_set_cursor(TERMINAL_HEIGHT - 5, 10);
    if (generator->edit_period) {
        ansi_send_text("Editing period!", &ansi_red_bold_conf);
    } else if (generator->edit_repeat) {
        ansi_send_text("Editing repeat!", &ansi_red_bold_conf);
    } else if (generator->permanent_send) {
        ansi_send_text("Sending repeating pulses!", &ansi_red_bold_conf);
    }
    help_signal_generator();
}

void help_signal_generator(void) {
    if (global_var.signal_generator->edit_period) {
        ansi_print_help_msg(
            "0-9: change period | X: delete period | E: stop edit", 0);
    } else if (global_var.signal_generator->edit_repeat) {
        ansi_print_help_msg(
            "0-9: change repeat | X: delete repeat | Y: stop edit", 0);
    } else {
        ansi_print_help_msg("D: permanent sending signals", 1);
        ansi_print_help_msg(
            "T: toggle output(High/Low)| S: send pulse| Y: edit repeat| E: "
            "edit period ",
            0);
    }
}

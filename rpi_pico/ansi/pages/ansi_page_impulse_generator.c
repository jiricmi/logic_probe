#include "ansi_page_impulse_generator.h"
#include <stdbool.h>
#include "ansi_abstraction_layer.h"
#include "ansi_ascii_text.h"

#include "ansi_pages.h"
#include "global_vars.h"
#include "utils.h"

extern global_vars_t global_var;

void ansi_render_impulse_generator_page(void) {
    global_var.current_page = ANSI_PAGE_IMPULSE_GENERATOR;
    ansi_render_border('%', "%", "");
    ansi_render_title(ASCII_LOGO_SIGNAL_GENERATOR, CYAN_TEXT);
    ansi_render_impulse_generator(&global_var.sig_gen_perif);
}

void ansi_render_impulse_generator(const sig_gen_t* generator) {
    char buff[50];
    char number_buff[15];
    ansi_set_cursor(9, 10);
    if (generator->mode == SIG_GEN_MODE_PULSE_UP) {
        ansi_send_text("UP  ", &ansi_default_conf);
    } else {
        ansi_send_text("DOWN", &ansi_default_conf);
    }
    ansi_set_cursor(10, 10);
    format_number_with_spaces(generator->period_us, number_buff);
    snprintf(buff, 50, "Pulse width (GPIO 17): %9.9s us", number_buff);
    ansi_send_text(buff, &ansi_default_conf);
    ansi_set_cursor(11, 10);
    format_number_with_spaces(generator->pwm_period, number_buff);
    snprintf(buff, 50, "Permanent send Period (GPIO 16): %4.4s ms",
             number_buff);
    ansi_send_text(buff, &ansi_default_conf);
    ansi_set_cursor(12, 10);

    ansi_set_cursor(TERMINAL_HEIGHT - 5, 10);
    if (generator->edit_period) {
        ansi_send_text("Editing period!", &ansi_red_bold_conf);
    } else if (generator->edit_pwm) {
        ansi_send_text("Editing permanent period!", &ansi_red_bold_conf);
    }
    help_signal_generator();
}

void help_signal_generator(void) {
    if (global_var.sig_gen_perif.edit_period) {
        ansi_print_help_msg(
            "0-9: change period | X: delete period | E: stop edit", 0);
    } else if (global_var.sig_gen_perif.edit_pwm) {
        ansi_print_help_msg(
            "0-9: change period | X: delete period | Y: stop edit", 0);
    } else {
        ansi_print_help_msg(
            "T: toggle output(High/Low) | S: send pulse | E: "
            "edit period ",
            0);
        ansi_print_help_msg(" Y: permanent period | D: start permanent send",
                            1);
    }
}

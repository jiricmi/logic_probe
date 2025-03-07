#include "ansi_page_impulse_generator.h"
#include "ansi_abstraction_layer.h"
#include "ansi_ascii_text.h"

#include "global_vars.h"
#include "main.h"

extern global_vars_t global_var;

void ansi_render_impulse_generator_page(void) {
    global_var.current_page = ANSI_PAGE_IMPULSE_GENERATOR;
    ansi_render_border('%', "%", "");
    ansi_render_title(ASCII_LOGO_SIGNAL_GENERATOR, CYAN_TEXT);
    ansi_render_impulse_generator(global_var.signal_generator);
    ansi_impulse_generator_render_hint();
}

void ansi_render_impulse_generator(const sig_generator_t* generator) {
    char buff[100];
    ansi_set_cursor(10, 10);
    snprintf(buff, 100, "Pulse width (A7): %4lu ms", generator->period);
    ansi_send_text(buff, &ansi_default_conf);
    GPIO_PinState state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);
    ansi_set_cursor(11, 10);
    if (state == GPIO_PIN_SET) {
        snprintf(buff, 100, "INPUT: HIGH");
    } else {
        snprintf(buff, 100, "INPUT: LOW ");
    }
    ansi_send_text(buff, &ansi_default_conf);
}

void ansi_impulse_generator_render_hint(void) {
    ansi_set_cursor(TERMINAL_HEIGHT - 2, 4);
    ansi_send_text("a,d,g - decrease ", &ansi_default_conf);
    ansi_set_cursor(TERMINAL_HEIGHT - 2, 21);
    ansi_send_text("s,f,h - increase ", &ansi_default_conf);
    ansi_set_cursor(TERMINAL_HEIGHT - 2, 45);
    ansi_send_text("w - send ", &ansi_default_conf);
}

#include "ansi_page_channel_settings.h"

#include "adc_control.h"
#include "ansi_abstraction_layer.h"
#include "ansi_ascii_text.h"
#include "ansi_pages.h"
#include "global_vars.h"
#include "utils.h"

extern global_vars_t global_var;

void ansi_render_channel_menu(void) {
    const unsigned int center = TERMINAL_WIDTH / 2 - 9;
    unsigned int row = 13;

    for (int i = 1; i < NUM_CHANNELS; ++i) {
        ansi_text_config_t button_conf = {"", RED_BG, 0};
        char label[20];
        char num = itocd(i);
        char pin = itocd((int)global_var.adc_vars->pin[i]);
        snprintf(label, sizeof(label), "%c] CHANNEL %c (PA%c)", num, num, pin);
        button_conf.bg_color =
            (global_var.adc_vars->channel_state_unapplied[i] ? GREEN_BG
                                                             : RED_BG);
        ansi_render_button(label, row, center, &button_conf);
        ++row;
    }

    if (!global_var.adc_vars->applied) {
        ansi_set_cursor(row + 2, center - 12);
        ansi_text_config_t text_conf = {RED_TEXT, "", 0};
        ansi_send_text("Change was made! Press S to save settings.",
                       &text_conf);
    } else {
        ansi_clear_line(row + 2, 1);
    }
    ansi_clear_format();
}

void ansi_render_channel_settings(void) {
    global_var.current_page = ANSI_PAGE_CHANNEL_SETTINGS;
    ansi_render_border('#', "#", "");
    ansi_render_title(ASCII_LOGO_CHANNEL, GREEN_TEXT);
    ansi_render_channel_menu();
    ansi_home_cursor();
}

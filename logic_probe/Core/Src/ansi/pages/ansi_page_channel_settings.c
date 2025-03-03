#include "ansi_page_channel_settings.h"

#include "adc_control.h"
#include "ansi_abstraction_layer.h"
#include "ansi_ascii_text.h"
#include "ansi_pages.h"
#include "utils.h"

extern ansi_page_type_t current_page;
extern adc_channels* adc1_ch;

static const ansi_text_config_t default_config = {"", "", 0};

void ansi_render_channel_menu(void) {
    const unsigned int center = TERMINAL_WIDTH / 2 - 9;
    unsigned int row = 13;

    for (int i = 0; i < NUM_CHANNELS; ++i) {
        ansi_text_config_t button_conf = {"", RED_BG, 0};
        char label[20];
        char num = itocd(i + 1);
        char pin = itocd((int)adc1_ch->pin[i]);
        snprintf(label, sizeof(label), "%c] CHANNEL %c (PA%c)", num, num, pin);
        button_conf.bg_color =
            (adc1_ch->channel_unapplied[i] ? GREEN_BG : RED_BG);
        ansi_render_button(label, row, center, &button_conf);
        ++row;
    }

    if (!adc1_ch->applied) {
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
    current_page = ANSI_PAGE_CHANNEL_SETTINGS;
    ansi_render_border('#', "#", "");
    ansi_render_title(ASCII_LOGO_CHANNEL, GREEN_TEXT);
    ansi_render_channel_menu();
    ansi_home_cursor();
}

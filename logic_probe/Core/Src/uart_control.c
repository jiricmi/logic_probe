#include "uart_control.h"
#include "adc_control.h"
#include "ansi_abstraction_layer.h"
#include "ansi_pages.h"
#include "utils.h"

extern adc_channels* adc1_ch;

void get_current_control(void) {
    switch (current_page) {
        case MAIN_PAGE:
            control_main_page();
            break;
        case CHANNEL_PAGE:
            control_channel_set_page();
            break;
        case VOLTAGE_PAGE:
            control_voltage_page();
            break;
        default:
            control_main_page();
    }
}

void control_main_page(void) {
    switch (received_char) {
        case 'v':
        case 'V':
            ansi_clear_terminal();
            ansi_voltage_page();
            break;
        case 'c':
        case 'C':
            ansi_clear_terminal();
            ansi_channel_set_page();
    }
}

void control_channel_set_page(void) {
    switch (received_char) {
        case 'q':
        case 'Q':
            adc_remove_unapplied_channels(adc1_ch);
            ansi_clear_terminal();
            ansi_main_page();
            break;

        case '1':
        case '2':
        case '3':
        case '4': {
            int num = cdtoi(received_char);

            if (num == -1) {
                // todo: handle error
            }
            flip_adc_unapplied_channel(adc1_ch, (size_t)num);
            generate_channel_menu();
            break;
        }
        case 's':
        case 'S':
            adc_apply_channels(adc1_ch);
            generate_channel_menu();
    }
}

void control_voltage_page(void) {
    switch (received_char) {
        case 'q':
        case 'Q':
            ansi_clear_terminal();
            ansi_main_page();
            break;
    }
}

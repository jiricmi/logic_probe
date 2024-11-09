#include "uart_control.h"
#include "adc_control.h"
#include "ansi_abstraction_layer.h"
#include "ansi_pages.h"

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
            ansi_clear_terminal();
            ansi_main_page();
            break;
        case '1':
            adc1_ch->channel_1 = !adc1_ch->channel_1;
            adc1_ch->applied = false;
            generate_channel_menu();
            break;
        case '2':
            adc1_ch->channel_2 = !adc1_ch->channel_2;
            adc1_ch->applied = false;
            generate_channel_menu();
            break;
        case '3':
            adc1_ch->channel_3 = !adc1_ch->channel_3;
            adc1_ch->applied = false;
            generate_channel_menu();
            break;
        case '4':
            adc1_ch->channel_4 = !adc1_ch->channel_4;
            adc1_ch->applied = false;
            generate_channel_menu();
            break;
        case 's':
        case 'S':
            adc1_ch->applied = true;
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

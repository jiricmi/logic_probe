#include "uart_control.h"
#include "adc_control.h"
#include "ansi_abstraction_layer.h"
#include "ansi_pages.h"
#include "signal_detector.h"
#include "signal_generator.h"
#include "utils.h"

extern ansi_page_type_t current_page;
extern unsigned char received_char;
extern adc_channels* adc1_ch;
extern sig_detector_t signal_detector;
extern sig_gen_t signal_generator;

void get_current_control(void) {
    switch (current_page) {
        case ANSI_PAGE_MAIN:
            control_main_page();
            break;
        case ANSI_PAGE_CHANNEL_SETTINGS:
            control_channel_set_page();
            break;
        case ANSI_PAGE_VOLTAGE_MEASURE:
            control_voltage_page();
            break;
        case ANSI_PAGE_FREQUENCY_READER:
            control_frequency_reader_page();
            break;
        case ANSI_PAGE_IMPULSE_GENERATOR:
            control_impulse_generator_page();
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
            ansi_set_current_page(ANSI_PAGE_VOLTAGE_MEASURE);
            break;
        case 'c':
        case 'C':
            ansi_clear_terminal();
            ansi_set_current_page(ANSI_PAGE_CHANNEL_SETTINGS);
            break;
        case 'f':
        case 'F':
            ansi_clear_terminal();
            ansi_set_current_page(ANSI_PAGE_FREQUENCY_READER);
            break;
        case 'g':
        case 'G':
            ansi_clear_terminal();
            ansi_set_current_page(ANSI_PAGE_IMPULSE_GENERATOR);
    }
}

void control_channel_set_page(void) {
    switch (received_char) {
        case 'q':
        case 'Q':
            adc_remove_unapplied_channels(adc1_ch);
            ansi_clear_terminal();
            ansi_set_current_page(ANSI_PAGE_MAIN);
            break;

        case '1':
        case '2':
        case '3':
        case '4': {
            int num = cdtoi(received_char);

            if (num == -1) {
                // TODO: handle error
            }
            flip_adc_unapplied_channel(adc1_ch, (size_t)num);
            ansi_render_current_page();
            break;
        }
        case 's':
        case 'S':
            adc_apply_channels(adc1_ch);
            ansi_render_current_page();
    }
}

void control_voltage_page(void) {
    switch (received_char) {
        case 'q':
        case 'Q':
            ansi_clear_terminal();
            ansi_set_current_page(ANSI_PAGE_MAIN);
            break;
    }
}

void control_frequency_reader_page(void) {
    switch (received_char) {
        case 'q':
        case 'Q':
            ansi_clear_terminal();
            ansi_set_current_page(ANSI_PAGE_MAIN);
            break;
        case 'm':
        case 'M':
            detector_change_mode(&signal_detector);
            break;
        case 't':
        case 'T':
            detector_change_sample_time(&signal_detector);
            break;
        case 'd':
        case 'D':
            signal_detector.p = false;
            break;
    }
}

void control_impulse_generator_page(void) {
    switch (received_char) {
        case 'q':
        case 'Q':
            ansi_clear_terminal();
            ansi_set_current_page(ANSI_PAGE_MAIN);
            break;
        case 'a':
        case 'A':
            sig_gen_change_period(&signal_generator, -1);
            break;
        case 's':
        case 'S':
            sig_gen_change_period(&signal_generator, 1);
            break;
        case 'd':
        case 'D':
            sig_gen_change_period(&signal_generator, -10);
            break;
        case 'f':
        case 'F':
            sig_gen_change_period(&signal_generator, 10);
            break;
        case 'g':
        case 'G':
            sig_gen_change_period(&signal_generator, -100);
            break;
        case 'h':
        case 'H':
            sig_gen_change_period(&signal_generator, 100);
            break;
        case 'w':
        case 'W':
            sig_gen_toggle_pulse(&signal_generator, false);
            break;
        case 't':
        case 'T':
            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7);
            break;
        case 'y':
        case 'Y':
            sig_gen_toggle_pulse(&signal_generator, true);
            break;
    }
}

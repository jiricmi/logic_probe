#include "uart_control.h"
#include "ansi_pages.h"

void get_current_control(void) {
    switch (current_page) {
        case 0:
            control_main_page();
            break;
        case 1:
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
            ansi_voltage_page();
            break;
    }
}

void control_voltage_page(void) {
    switch (received_char) {
        case 'q':
        case 'Q':
            ansi_main_page();
    }
}

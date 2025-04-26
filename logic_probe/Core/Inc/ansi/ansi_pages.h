#ifndef ANSI_PAGES_H
#define ANSI_PAGES_H
#include <stdint.h>

#define BASE_TEXT_BUFF_LEN 100
#define NUM_BUFF_LEN 15

typedef enum {
    ANSI_PAGE_MAIN,
    ANSI_PAGE_MAIN_ADVANCED,
    ANSI_PAGE_VOLTAGE_MEASURE,
    ANSI_PAGE_FREQUENCY_READER,
    ANSI_PAGE_IMPULSE_GENERATOR,
    ANSI_PAGE_NEOPIXEL_MEASURE,
    ANSI_PAGE_SHIFT_REGISTER,
    ANSI_PAGE_UART,
    ANSI_PAGE_I2C,
    ANSI_PAGE_SPI,
} ansi_page_type_t;

void ansi_render_current_page(void);

void ansi_set_current_page(ansi_page_type_t page);

void ansi_print_help_msg(const char* msg, const uint8_t row_offset);
#endif

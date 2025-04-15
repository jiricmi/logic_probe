#ifndef ANSI_PAGES_H
#define ANSI_PAGES_H

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
} ansi_page_type_t;

void ansi_render_current_page(void);

void ansi_set_current_page(ansi_page_type_t page);
#endif

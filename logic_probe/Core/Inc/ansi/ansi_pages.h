#ifndef ANSI_PAGES_H
#define ANSI_PAGES_H

typedef enum {
    ANSI_PAGE_MAIN,
    ANSI_PAGE_VOLTAGE_MEASURE,
    ANSI_PAGE_FREQUENCY_READER,
    ANSI_PAGE_IMPULSE_GENERATOR,
    ANSI_PAGE_NEOPIXEL_MEASURE,
} ansi_page_type_t;

void ansi_render_current_page(void);

void ansi_set_current_page(ansi_page_type_t page);
#endif

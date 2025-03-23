#ifndef EXTERN_BUTTON_H
#define EXTERN_BUTTON_H
#include <stdint.h>
#include "main.h"

#define BUTTON_PIN GPIO_PIN_13

#define SHORT_PRESS_TIME 30
#define LONG_PRESS_TIME 1000
#define DOUBLE_PRESS_TIME 300

typedef struct {
    _Bool is_pressed;
    _Bool short_press;
    _Bool double_press;
    _Bool long_press;
    uint32_t rise_edge_time;
    uint32_t fall_edge_time;
    uint32_t last_short_button_time;
    uint16_t pin;
} button_data_t;

void init_button_data(button_data_t* data);

void extern_button_check_press(button_data_t* data);

void extern_button_handle_short(button_data_t* data);

void extern_button_handle_long(button_data_t* data);

void extern_button_handle_double(button_data_t* data);
#endif

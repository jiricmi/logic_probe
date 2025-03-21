#include "extern_button.h"
#include <stdbool.h>
#include <string.h>

void init_button_data(button_data_t* data) {
    memset(data, 0, sizeof(*data));
    data->pin = BUTTON_PIN;
}

void extern_button_check_press(button_data_t* data) {
    uint32_t time = data->fall_edge_time - data->rise_edge_time;

    if (time > SHORT_PRESS_TIME && time < LONG_PRESS_TIME) {
        data->short_press = true;
        data->long_press = false;
    } else if (time > LONG_PRESS_TIME) {
        data->long_press = true;
        data->short_press = false;
    } else {
        data->short_press = false;
        data->long_press = false;
    }
}

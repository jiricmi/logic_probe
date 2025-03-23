#include "extern_button.h"
#include <stdbool.h>
#include <string.h>
#include "global_vars.h"

extern global_vars_t global_var;

void init_button_data(button_data_t* data) {
    memset(data, 0, sizeof(*data));
    data->pin = BUTTON_PIN;
}

void extern_button_check_press(button_data_t* data) {
    uint32_t time = data->fall_edge_time - data->rise_edge_time;

    if (time > SHORT_PRESS_TIME && time < LONG_PRESS_TIME) {
        data->short_press = true;
        data->long_press = false;
        uint32_t curr_time = HAL_GetTick();

        if (curr_time - data->last_short_button_time < DOUBLE_PRESS_TIME &&
            !data->double_press) {
            data->double_press = true;
            data->short_press = false;
        } else {
            data->last_short_button_time = curr_time;
        }

    } else if (time > LONG_PRESS_TIME && !data->long_press) {
        data->long_press = true;
        data->short_press = false;
    }
}

void extern_button_handle_short(button_data_t* data) {
    uint32_t time = HAL_GetTick();

    if (time - data->last_short_button_time < DOUBLE_PRESS_TIME) {
        return;
    }

    switch (global_var.local_state) {
        case LOCAL_STATE_LOGIC_PROBE:
        case LOCAL_STATE_VOLTMETER_PROBE:
            if (global_var.local_substate == LOCAL_SUBSTATE_CHANNEL_1) {
                global_var.local_substate = LOCAL_SUBSTATE_CHANNEL_2;
                neopixel_send_color(global_var.visual_output, NEOPIXEL_YELLOW);
            } else {
                global_var.local_substate = LOCAL_SUBSTATE_CHANNEL_1;
                neopixel_send_color(global_var.visual_output, NEOPIXEL_PURPLE);
            }
            HAL_Delay(1000);
            break;
        case LOCAL_STATE_OUTPUT:
            if (global_var.local_substate == LOCAL_SUBSTATE_CHANNEL_1) {
                HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
            } else {
                HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);
            }
            break;
        default:
            break;
    }
    data->short_press = false;
}

void extern_button_handle_long(button_data_t* data) {
    switch (global_var.local_state) {
        case LOCAL_STATE_LOGIC_PROBE:
            global_var.local_substate = LOCAL_SUBSTATE_CHANNEL_1;
            global_var.local_state = LOCAL_STATE_VOLTMETER_PROBE;
            neopixel_send_color(global_var.visual_output, NEOPIXEL_ORANGE);
            break;
        case LOCAL_STATE_VOLTMETER_PROBE:
            global_var.local_substate = LOCAL_SUBSTATE_CHANNEL_1;
            global_var.local_state = LOCAL_STATE_OUTPUT;
            neopixel_send_color(global_var.visual_output, NEOPIXEL_BLUE);
            break;
        case LOCAL_STATE_OUTPUT:
            global_var.local_state = LOCAL_STATE_PULSEUP;
            neopixel_send_color(global_var.visual_output, NEOPIXEL_WHITE);
            break;
        case LOCAL_STATE_PULSEUP:
            global_var.local_state = LOCAL_STATE_PULSEDOWN;
            neopixel_send_color(global_var.visual_output, NEOPIXEL_PURPLE);
            break;
        case LOCAL_STATE_PULSEDOWN:
            global_var.local_substate = LOCAL_SUBSTATE_CHANNEL_1;
            global_var.local_state = LOCAL_STATE_LOGIC_PROBE;
            neopixel_send_color(global_var.visual_output, NEOPIXEL_AQUA);
            break;

        default:
            break;
    }
    HAL_Delay(1000);
    data->long_press = false;
}

void extern_button_handle_double(button_data_t* data) {
    switch (global_var.local_state) {
        case LOCAL_STATE_OUTPUT:
            if (global_var.local_substate == LOCAL_SUBSTATE_CHANNEL_1) {
                global_var.local_substate = LOCAL_SUBSTATE_CHANNEL_2;
                neopixel_send_color(global_var.visual_output, NEOPIXEL_YELLOW);
            } else {
                global_var.local_substate = LOCAL_SUBSTATE_CHANNEL_1;
                neopixel_send_color(global_var.visual_output, NEOPIXEL_PURPLE);
            }

            break;
        default:
            break;
    }
    HAL_Delay(1000);
    data->double_press = false;
}

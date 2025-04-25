#include "extern_button.h"
#include <stdbool.h>
#include <string.h>
#include "global_structs.h"
#include "global_vars.h"
#include "signal_generator.h"

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
    visual_output_t* visual_output = global_var.visual_output;
    sig_generator_t* signal_generator = global_var.signal_generator;
    local_substate_t* substate = &global_var.local_substate;

    if (time - data->last_short_button_time < DOUBLE_PRESS_TIME) {
        return;
    }

    switch (global_var.local_state) {
        case LOCAL_STATE_LOGIC_PROBE:
            if (substate == LOCAL_SUBSTATE_CHANNEL_1) {
                *substate = LOCAL_SUBSTATE_CHANNEL_2;
                neopixel_send_color(visual_output, NEOPIXEL_YELLOW);
            } else {
                *substate = LOCAL_SUBSTATE_CHANNEL_1;
                neopixel_send_color(visual_output, NEOPIXEL_PURPLE);
            }
            HAL_Delay(1000);
            break;
        case LOCAL_STATE_OUTPUT:
            if (substate == LOCAL_SUBSTATE_CHANNEL_1) {
                HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
            } else {
                HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);
            }
            break;
        case LOCAL_STATE_PULSING:
            signal_generator->local_pulsing = !signal_generator->local_pulsing;
            if (signal_generator->local_pulsing) {
                neopixel_send_color(visual_output, NEOPIXEL_WHITE);
            } else {
                neopixel_send_color(visual_output, NEOPIXEL_NONE);
            }
            break;
        default:
            break;
    }
    data->short_press = false;
}

void extern_button_handle_long(button_data_t* data) {
    local_state_t* state = &global_var.local_state;
    local_substate_t* substate = &global_var.local_substate;
    visual_output_t* visual_output = global_var.visual_output;
    switch (global_var.local_state) {
        case LOCAL_STATE_LOGIC_PROBE:
            *substate = LOCAL_SUBSTATE_CHANNEL_1;
            *state = LOCAL_STATE_OUTPUT;
            neopixel_send_color(visual_output, NEOPIXEL_ORANGE);
            break;
        case LOCAL_STATE_OUTPUT:
            *state = LOCAL_STATE_PULSING;
            neopixel_send_color(visual_output, NEOPIXEL_WHITE);
            break;
        case LOCAL_STATE_PULSING:
            *state = LOCAL_STATE_PULSEUP;
            neopixel_send_color(visual_output, NEOPIXEL_BLUE);
            break;
        case LOCAL_STATE_PULSEUP:
            *state = LOCAL_STATE_PULSEDOWN;
            neopixel_send_color(visual_output, NEOPIXEL_PURPLE);
            break;
        case LOCAL_STATE_PULSEDOWN:
            *substate = LOCAL_SUBSTATE_CHANNEL_1;
            *state = LOCAL_STATE_LOGIC_PROBE;
            neopixel_send_color(visual_output, NEOPIXEL_AQUA);
            break;

        default:
            break;
    }
    HAL_Delay(1000);
    data->long_press = false;
}

void extern_button_handle_double(button_data_t* data) {
    local_substate_t* substate = &global_var.local_substate;

    switch (global_var.local_state) {
        case LOCAL_STATE_OUTPUT:
            if (*substate == LOCAL_SUBSTATE_CHANNEL_1) {
                *substate = LOCAL_SUBSTATE_CHANNEL_2;
                neopixel_send_color(global_var.visual_output, NEOPIXEL_YELLOW);
            } else {
                *substate = LOCAL_SUBSTATE_CHANNEL_1;
                neopixel_send_color(global_var.visual_output, NEOPIXEL_PURPLE);
            }

            break;
        default:
            break;
    }
    HAL_Delay(1000);
    data->double_press = false;
}

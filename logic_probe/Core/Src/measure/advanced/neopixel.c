#include "advanced/neopixel.h"
#include <stdbool.h>
#include <string.h>

void init_neopixel_measure(neopixel_measure_t* data, TIM_HandleTypeDef* htim) {
    memset(data, 0, sizeof(*data));
    data->htim = htim;
    data->rgb_send[3] = 1;
}

void neopixel_read_parse(neopixel_measure_t* data) {
    uint32_t pulse_width;
    uint8_t bits_detected[NEOPIXEL_BIT_LEN];
    int8_t start_index = -1;
    uint8_t prev_measure = NEOPIXEL_DATA_LEN - 1;
    for (uint8_t i = 0; i < NEOPIXEL_DATA_LEN; ++i) {
        uint32_t curr_rise_edge = data->rise_edge[i];
        uint32_t prev_fall_edge = data->fall_edge[prev_measure];
        uint64_t width;
        if (curr_rise_edge < prev_fall_edge) {
            width = (curr_rise_edge + 0xFFFFFFFF) - prev_fall_edge;
        } else {
            width = curr_rise_edge - prev_fall_edge;
        }

        if (width > NEOPIXEL_RESET) {
            start_index = i;
            break;
        }
    }

    if (start_index < 0) {
        return;
    }

    for (uint8_t i = 0; i < NEOPIXEL_DATA_LEN; ++i) {
        uint8_t index = i + start_index;

        if (index >= NEOPIXEL_DATA_LEN) {
            index -= NEOPIXEL_DATA_LEN;
        }

        pulse_width = data->fall_edge[i] - data->rise_edge[i];

        if (pulse_width >= NEOPIXEL_MIN_LOW &&
            pulse_width <= NEOPIXEL_MAX_LOW) {
            bits_detected[i] = 0;
        } else if (pulse_width >= NEOPIXEL_MIN_HIGH &&
                   pulse_width <= NEOPIXEL_MAX_HIGH) {
            bits_detected[i] = 1;
        } else {
            return;
        }
    }

    for (uint8_t element = 0; element < RGB_ELEMENTS; ++element) {
        data->rgb[element] = 0;
        for (uint8_t bit_index = 0; bit_index < NEOPIXEL_ELEMENT_SIZE;
             ++bit_index) {
            uint8_t global_bit_pos =
                element * NEOPIXEL_ELEMENT_SIZE + bit_index;
            if (global_bit_pos >= NEOPIXEL_BIT_LEN)
                break;
            data->rgb[element] |= bits_detected[global_bit_pos]
                                  << (7 - bit_index);
        }
    }
}

void neopixel_read_start(neopixel_measure_t* data) {
    HAL_TIM_IC_Start_DMA(data->htim, TIM_CHANNEL_1, data->rise_edge,
                         NEOPIXEL_DATA_LEN);

    HAL_TIM_IC_Start_DMA(data->htim, TIM_CHANNEL_2, data->fall_edge,
                         NEOPIXEL_DATA_LEN);
}

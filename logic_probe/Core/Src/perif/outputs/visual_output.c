#include "visual_output.h"
#include <string.h>
#include "main.h"

void init_visual_output(visual_output_t* visual_output,
                        TIM_HandleTypeDef* htim) {
    memset(visual_output, 0, sizeof(*visual_output));

    visual_output->neopixel_tim = htim;
}

void neopixel_send_color(visual_output_t* visual_output,
                         neopixel_color_enum_t color_type) {
    neopixel_color_t color;
    neopixel_get_color(&color, color_type);

    neopixel_set_color(visual_output, &color);

    HAL_TIM_PWM_Start_DMA(visual_output->neopixel_tim, TIM_CHANNEL_3,
                          visual_output->neopixel_data, NEOPIXEL_DATA_SIZE);
}

void neopixel_set_color(visual_output_t* visual_output,
                        neopixel_color_t* color) {
    uint32_t data = (color->b << 16) | color->g << 8 | color->r;
    for (uint8_t i = 0; i < NEOPIXEL_DATA_SIZE - 1; ++i) {
        if (data & (1 << i)) {
            visual_output->neopixel_data[i] = 54;
        } else {
            visual_output->neopixel_data[i] = 26;
        }
    }
    visual_output->neopixel_data[NEOPIXEL_DATA_SIZE - 1] = 0;
}

void neopixel_get_color(neopixel_color_t* color,
                        neopixel_color_enum_t color_type) {
    switch (color_type) {
        case NEOPIXEL_RED:
            color->r = 255;
            color->g = 0;
            color->b = 0;
            break;
        case NEOPIXEL_BLUE:
            color->r = 0;
            color->g = 0;
            color->b = 255;
            break;
        case NEOPIXEL_GREEN:
            color->r = 0;
            color->g = 255;
            color->b = 0;
            break;
        case NEOPIXEL_NONE:
            color->r = 0;
            color->g = 0;
            color->b = 0;
            break;
        case NEOPIXEL_WHITE:
            color->r = 255;
            color->g = 255;
            color->b = 255;
            break;

        default:
            break;
    }
}

void neopixel_startup_effect(visual_output_t* visual_output) {
    neopixel_send_color(visual_output, NEOPIXEL_WHITE);
    HAL_Delay(1000);
    neopixel_send_color(visual_output, NEOPIXEL_RED);
    HAL_Delay(1000);
    neopixel_send_color(visual_output, NEOPIXEL_GREEN);
    HAL_Delay(1000);
    neopixel_send_color(visual_output, NEOPIXEL_BLUE);
    HAL_Delay(1000);
}

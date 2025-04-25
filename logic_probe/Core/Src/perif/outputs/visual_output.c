#include "visual_output.h"
#include <string.h>
#include "adc_control.h"
#include "global_structs.h"
#include "signal_detector.h"

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
}

void neopixel_set_color(visual_output_t* visual_output,
                        neopixel_color_t* color) {
    uint32_t data = (color->r << 16) | color->g << 8 | color->b;
    for (uint8_t i = 3; i < NEOPIXEL_DATA_SIZE - 3; ++i) {
        if (data & (1 << (NEOPIXEL_DATA_SIZE - 6 - i))) {
            visual_output->neopixel_data[i] = 54;
        } else {
            visual_output->neopixel_data[i] = 26;
        }
    }

    visual_output->neopixel_data[0] = 0;
    memset(visual_output->neopixel_data, 0, 3);
    memset(visual_output->neopixel_data + NEOPIXEL_DATA_SIZE - 3, 0, 3);

    HAL_TIM_PWM_Start_DMA(visual_output->neopixel_tim, TIM_CHANNEL_3,
                          visual_output->neopixel_data, NEOPIXEL_DATA_SIZE);
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
        case NEOPIXEL_PURPLE:
            color->r = 255;
            color->g = 0;
            color->b = 255;
            break;
        case NEOPIXEL_YELLOW:
            color->r = 255;
            color->g = 255;
            color->b = 0;
            break;
        case NEOPIXEL_ORANGE:
            color->r = 255;
            color->g = 159;
            color->b = 0;
            break;
        case NEOPIXEL_AQUA:
            color->r = 0;
            color->g = 255;
            color->b = 245;
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

void neopixel_show_probe_state(visual_output_t* visual_output,
                               probe_state_t probe_state) {
    if (probe_state == PROBE_STATE_HIGH) {
        neopixel_send_color(visual_output, NEOPIXEL_GREEN);
    } else if (probe_state == PROBE_STATE_LOW) {
        neopixel_send_color(visual_output, NEOPIXEL_RED);
    } else {
        neopixel_send_color(visual_output, NEOPIXEL_NONE);
    }
}

void neopixel_show_voltmeter_state(visual_output_t* visual_output,
                                   local_substate_t state,
                                   uint32_t voltage) {
    uint32_t light;
    if (state == LOCAL_SUBSTATE_CHANNEL_1) {
        light = voltage / NEOPIXEL_DIV_VOLTAGE_CONST;
        neopixel_color_t color = {light, 0, light};
        neopixel_set_color(visual_output, &color);
    } else {
        light = voltage / NEOPIXEL_DIV_VOLTAGE_CONST;
        neopixel_color_t color = {light, light, 0};
        neopixel_set_color(visual_output, &color);
    }
}

void neopixel_show_output_state(visual_output_t* visual_output,
                                local_substate_t state) {
    if (state == LOCAL_SUBSTATE_CHANNEL_1) {
        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)) {
            neopixel_send_color(visual_output, NEOPIXEL_PURPLE);
        } else {
            neopixel_send_color(visual_output, NEOPIXEL_NONE);
        }
    } else {
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)) {
            neopixel_send_color(visual_output, NEOPIXEL_YELLOW);
        } else {
            neopixel_send_color(visual_output, NEOPIXEL_NONE);
        }
    }
}

void neopixel_show_pulse_detection(visual_output_t* visual_output,
                                   sig_detector_t* sig_det) {
    if (sig_det->mode == DETECTOR_MODE_PULSE_UP) {
        neopixel_send_color(visual_output, NEOPIXEL_PURPLE);
    } else {
        neopixel_send_color(visual_output, NEOPIXEL_YELLOW);
    }
    HAL_Delay(1000);
    neopixel_send_color(visual_output, NEOPIXEL_NONE);
}

#ifndef VISUAL_OUTPUT_H
#define VISUAL_OUTPUT_H

#include <stdint.h>
#include "adc_control.h"
#include "main.h"
#include "signal_detector.h"

#define NEOPIXEL_DATA_SIZE 30
#define NEOPIXEL_DIV_VOLTAGE_CONST 13

typedef struct {
    TIM_HandleTypeDef* neopixel_tim;
    uint16_t neopixel_data[NEOPIXEL_DATA_SIZE];
} visual_output_t;

typedef struct {
    uint16_t r;
    uint16_t g;
    uint16_t b;
} neopixel_color_t;

typedef enum {
    NEOPIXEL_RED,
    NEOPIXEL_BLUE,
    NEOPIXEL_GREEN,
    NEOPIXEL_NONE,
    NEOPIXEL_WHITE,
    NEOPIXEL_PURPLE,
    NEOPIXEL_YELLOW,
    NEOPIXEL_ORANGE,
    NEOPIXEL_AQUA,

} neopixel_color_enum_t;

void init_visual_output(visual_output_t* visual_output,
                        TIM_HandleTypeDef* htim);

void neopixel_send_color(visual_output_t* visual_output,
                         neopixel_color_enum_t color_type);

void neopixel_set_color(visual_output_t* visual_output,
                        neopixel_color_t* color);

void neopixel_get_color(neopixel_color_t* color,
                        neopixel_color_enum_t color_type);

void neopixel_startup_effect(visual_output_t* visual_output);

void neopixel_show_probe_state(visual_output_t* visual_output,
                               probe_state_t probe_state);

void neopixel_show_voltmeter_state(visual_output_t* visual_output,
                                   local_substate_t state,
                                   uint32_t voltage);

void neopixel_show_output_state(visual_output_t* visual_output,
                                local_substate_t state);

void neopixel_show_pulse_detection(visual_output_t* visual_output,
                                   sig_detector_t* sig_det);
#endif

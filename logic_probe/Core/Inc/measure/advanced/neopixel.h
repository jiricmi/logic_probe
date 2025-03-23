#ifndef NEOPIXEL_H
#define NEOPIXEL_H
#include <stdint.h>
#include "main.h"

#define NEOPIXEL_DATA_LEN 24
#define NEOPIXEL_BIT_LEN 24
#define NEOPIXEL_ELEMENT_SIZE 8

#define RGB_ELEMENTS 3

// 40Mhz frequency
#define NEOPIXEL_MIN_LOW 13
#define NEOPIXEL_MAX_LOW 32
#define NEOPIXEL_MIN_HIGH 36
#define NEOPIXEL_MAX_HIGH 54

#define NEOPIXEL_RESET 3200

typedef struct {
    TIM_HandleTypeDef* htim;
    uint32_t rise_edge[NEOPIXEL_DATA_LEN];
    uint32_t fall_edge[NEOPIXEL_DATA_LEN];
    _Bool failed_to_detect;
    _Bool detected;
    uint8_t rgb[RGB_ELEMENTS];

} neopixel_measure_t;

void init_neopixel_measure(neopixel_measure_t* data, TIM_HandleTypeDef* htim);

void neopixel_read_parse(neopixel_measure_t* data);

void neopixel_read_start(neopixel_measure_t* data);
#endif

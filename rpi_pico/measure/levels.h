#ifndef LEVELS_H
#define LEVELS_H
#include <stdint.h>
static const uint8_t LED_PINS[] = {18, 19, 20};

void init_level_gpio(void);

void deinit_level_gpio(void);

#endif

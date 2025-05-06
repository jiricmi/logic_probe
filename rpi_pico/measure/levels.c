#include "levels.h"
#include "global_vars.h"
#include "hardware/gpio.h"

extern global_vars_t global_var;

void init_level_gpio(void) {
    for (int i = 0; i < sizeof(LED_PINS) / sizeof(LED_PINS[0]); i++) {
        gpio_init(LED_PINS[i]);
        gpio_set_dir(LED_PINS[i], GPIO_OUT);
        gpio_put(LED_PINS[i], global_var.gpio_level_state[i]);
    }
}

void deinit_level_gpio(void) {
    for (int i = 0; i < sizeof(LED_PINS) / sizeof(LED_PINS[0]); i++) {
        gpio_deinit(LED_PINS[i]);
    }
}

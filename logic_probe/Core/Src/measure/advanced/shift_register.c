#include "advanced/shift_register.h"
#include <stdbool.h>
#include <string.h>
#include "main.h"

void init_shift_register(shift_register_t* shift_register) {
    memset(shift_register, 0, sizeof(*shift_register));
}

void shift_register_send_signal(shift_register_t* shift_register) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
    for (uint8_t i = 0; i < SHIFT_REGISTER_BIT_COUNT; ++i) {
        if (shift_register->bits[i]) {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
        } else {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
        }
        HAL_Delay(SHIFT_REGISTER_LATCH);
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
        HAL_Delay(SHIFT_REGISTER_LATCH);
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
        HAL_Delay(SHIFT_REGISTER_LATCH);
    }
    shift_register->ready_to_send = false;
}

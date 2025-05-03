#include "advanced/shift_register.h"
#include <stdbool.h>
#include <string.h>
#include "main.h"

void init_shift_register(shift_register_t* shift_register) {
    memset(shift_register, 0, sizeof(*shift_register));
}

void shift_register_check_signal(shift_register_t* shift_register) {
    if (shift_register->ready_to_send) {
        shift_register_send_signal(shift_register);
        shift_register->ready_to_send = false;
    } else if (shift_register->ready_to_send_bit) {
        shift_register_one_signal(shift_register);
        shift_register->ready_to_send_bit = false;
    } else if (shift_register->send_rclk) {
        shift_register_send_rclk();
        shift_register->send_rclk = 0;
    }
}

void shift_register_send_signal(shift_register_t* shift_register) {
    for (uint8_t i = 0; i < SHIFT_REGISTER_BIT_COUNT; ++i) {
        shift_register_send_one_signal(shift_register, i);
    }
    shift_register_send_rclk();
}

void shift_register_send_one_signal(shift_register_t* shift_register,
                                    const uint8_t index) {
    if (shift_register->bits[index]) {
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

void shift_register_send_rclk(void) {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
    HAL_Delay(SHIFT_REGISTER_LATCH);
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
}

void shift_register_one_signal(shift_register_t* shift_register) {
    shift_register_send_one_signal(shift_register, shift_register->index);

    shift_register->index++;
    if (shift_register->index >= 8) {
        shift_register->index = 0;
    }
}

#ifndef SHIFT_REGISTER_H
#define SHIFT_REGISTER_H
#include <stdint.h>

#define SHIFT_REGISTER_BIT_COUNT 8

#define SHIFT_REGISTER_LATCH 100

typedef struct {
    _Bool bits[SHIFT_REGISTER_BIT_COUNT];
    _Bool ready_to_send;
    _Bool send_rclk;
    uint8_t index;
    _Bool ready_to_send_bit;
} shift_register_t;

void init_shift_register(shift_register_t* shift_register);
void shift_register_send_signal(shift_register_t* shift_register);
void shift_register_send_one_signal(shift_register_t* shift_register,
                                    const uint8_t index);
void shift_register_send_rclk(void);
void shift_register_one_signal(shift_register_t* shift_register);
void shift_register_check_signal(shift_register_t* shift_register);
#endif

#ifndef SHIFT_REGISTER_H
#define SHIFT_REGISTER_H

#define SHIFT_REGISTER_BIT_COUNT 8

#define SHIFT_REGISTER_LATCH 50

typedef struct {
    _Bool bits[SHIFT_REGISTER_BIT_COUNT];
    _Bool ready_to_send;
} shift_register_t;

void init_shift_register(shift_register_t* shift_register);
void shift_register_send_signal(shift_register_t* shift_register);
#endif

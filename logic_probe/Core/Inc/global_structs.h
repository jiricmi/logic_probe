#ifndef GLOBAL_STRUCTS_H
#define GLOBAL_STRUCTS_H

typedef enum {
    DEV_STATE_NONE,
    DEV_STATE_VOLTMETER,
    DEV_STATE_OHMMETER,
    DEV_STATE_FREQUENCY_READ,
    DEV_STATE_PULSE_GEN,
    DEV_STATE_ADV_NEOPIXEL_READ,
    DEV_STATE_ADV_NEOPIXEL_WRITE
} dev_state_t;

typedef enum {
    LOCAL_STATE_LOGIC_PROBE,      // aqua
    LOCAL_STATE_VOLTMETER_PROBE,  // orange
    LOCAL_STATE_OUTPUT,           // blue
    LOCAL_STATE_PULSEUP,          // white
    LOCAL_STATE_PULSEDOWN,        // purple
} local_state_t;

typedef enum {
    LOCAL_SUBSTATE_CHANNEL_1,  // purple
    LOCAL_SUBSTATE_CHANNEL_2,  // yellow
} local_substate_t;

typedef enum {
    DEV_SETUP_UART,
    DEV_SETUP_LOCAL,
} dev_setup_t;

#endif

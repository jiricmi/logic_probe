#ifndef PROBE_USB_H
#define PROBE_USB_H
#include <pico/time.h>
#include <pico/util/queue.h>
#include "hardware/sync.h"

#define USB_CHAR_QUEUE_COUNT 32

typedef struct {
    queue_t received_chars;
    spin_lock_t* usb_lock;
    repeating_timer_t control_timer;

} usb_perif_t;

void usb_init(usb_perif_t* perif);

bool usb_handle_control(repeating_timer_t* rt);

void usb_service();

#endif

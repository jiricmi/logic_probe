#include "probe_usb.h"
#include <pico/multicore.h>
#include <pico/time.h>
#include "global_vars.h"
#include "pico/stdlib.h"
#include "pico/util/queue.h"
#include "tusb.h"
#include "uart_control.h"

extern global_vars_t global_var;

void usb_init(usb_perif_t* perif) {
    queue_init(&perif->received_chars, sizeof(char), USB_CHAR_QUEUE_COUNT);
    perif->usb_lock = spin_lock_instance(spin_lock_claim_unused(true));
    multicore_launch_core1(usb_service);

    add_repeating_timer_ms(10, usb_handle_control, NULL, &perif->control_timer);
}

bool usb_handle_control(repeating_timer_t* rt) {
    usb_perif_t* usb_perif = &global_var.usb_perif;
    char received_char;

    while (true) {
        uint32_t save = spin_lock_blocking(usb_perif->usb_lock);
        if (queue_is_empty(&usb_perif->received_chars)) {
            spin_unlock(usb_perif->usb_lock, save);
            break;
        }
        queue_try_remove(&usb_perif->received_chars, &received_char);
        spin_unlock(usb_perif->usb_lock, save);
        get_current_control(received_char);
    }
    return true;
}

void usb_service(void) {
    usb_perif_t* perif = &global_var.usb_perif;
    while (true) {
        tud_task();
        if (tud_cdc_available()) {
            char input_c;
            tud_cdc_read(&input_c, 1);
            uint32_t save = spin_lock_blocking(perif->usb_lock);
            queue_try_add(&perif->received_chars, &input_c);
            spin_unlock(perif->usb_lock, save);
        }
        sleep_ms(1);
    }
}

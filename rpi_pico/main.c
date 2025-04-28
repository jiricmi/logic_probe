#include "pico/multicore.h"
#include "pico/stdlib.h"
#include "pico/util/queue.h"
#include "tusb.h"

queue_t fronta_znaku;  // Fronta pro komunikaci

void core1_entry() {
    while (true) {
        tud_task();  // Zpracování USB (na Core0)
        if (tud_cdc_available()) {
            char znak;
            tud_cdc_read(&znak, 1);
            queue_try_add(&fronta_znaku, &znak);  // Odeslat do Core1
        }
    }
}

int main() {
    stdio_init_all();
    queue_init(&fronta_znaku, sizeof(char), 32);  // Fronta pro 32 znaků
    multicore_launch_core1(core1_entry);

    while (true) {
        tud_task();  // Zpracování USB (na Core0)
        if (tud_cdc_available()) {
            char znak;
            tud_cdc_read(&znak, 1);
            queue_try_add(&fronta_znaku, &znak);  // Odeslat do Core1
        }
    }
}

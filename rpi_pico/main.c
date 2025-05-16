#include "ansi_pages.h"
#include "global_vars.h"
#include "hardware/adc.h"
#include "loop.h"
#include "measure/adc_control.h"
#include "probe_usb.h"

extern global_vars_t global_var;
bool led_state = false;

void init_led() {
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
}

void setup() {
    stdio_init_all();
    adc_init();
    adc_set_temp_sensor_enabled(false);
    init_led();
    usb_init(&global_var.usb_perif);
    adc_init_struct(&global_var.adc_perif);
    sig_gen_init_struct(&global_var.sig_gen_perif);
}

int main() {
    setup();

    while (!global_var.booted) {
        ansi_render_welcome_message();
        sleep_ms(200);
        gpio_put(PICO_DEFAULT_LED_PIN, led_state = !led_state);
    }
    while (true) {
        dev_mode_run();
    }
}

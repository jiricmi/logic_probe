#include <stdio.h>
#include "global_vars.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "loop.h"
#include "measure/adc_control.h"
#include "pico/stdlib.h"
#include "probe_usb.h"
#include "signal_detection.h"

extern global_vars_t global_var;

void setup() {
    stdio_init_all();
    adc_init();
    adc_set_temp_sensor_enabled(false);
    usb_init(&global_var.usb_perif);
    adc_init_struct(&global_var.adc_perif);
    sig_gen_init_struct(&global_var.sig_gen_perif);
}

int main() {
    setup();
    while (true) {
        dev_mode_run();
    }
}

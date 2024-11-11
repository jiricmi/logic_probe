#include "adc_control.h"
#include <stdlib.h>
#include <string.h>

adc_channels* create_adc_channels(void) {
    adc_channels* adc_ch = (adc_channels*)malloc(sizeof(adc_channels));

    if (adc_ch == NULL) {
        // todo: error handle
    }

    for (size_t i = 0; i < NUM_CHANNELS; ++i) {
        adc_ch->channel[i] = false;
    }

    unsigned int pin_values[] = {0, 1, 4, 5}; // todo: lepe?
    memcpy(adc_ch->pin, pin_values, sizeof(pin_values));

    adc_ch->applied = true;
    adc_ch->count_active = count_active_channels(adc_ch);

    return adc_ch;
}

void flip_adc_channel(adc_channels* adc_ch, size_t channel) {
    if (channel == 0) {
        // todo: error
        return;
    }

    channel -= 1;
    adc_ch->channel[channel] = !adc_ch->channel[channel];
    adc_ch->count_active = count_active_channels(adc_ch);
    adc_ch->applied = false;
}

size_t count_active_channels(adc_channels* adc_ch) {
    size_t count = 0;
    for (size_t i = 0; i < NUM_CHANNELS; ++i) {
        if (adc_ch->channel[i]) {
            ++count;
        }
    }
    return count;
}

void setup_adc_channels(adc_channels* adc_ch) {
    unsigned int i = 0;
}

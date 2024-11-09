#include <stdlib.h>

#include "adc_control.h"

adc_channels* create_adc_channels(void) {
    adc_channels* adc_ch = (adc_channels*)malloc(sizeof(adc_channels));

    if (adc_ch == NULL) {
        // todo: error handle
    }
    adc_ch->channel_1 = false;
    adc_ch->channel_2 = false;
    adc_ch->channel_3 = false;
    adc_ch->channel_4 = false;

    return adc_ch;
}

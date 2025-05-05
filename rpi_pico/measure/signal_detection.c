#include "signal_detection.h"
#include "hardware/dma.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

void sig_det_frequecy_counter_init(timer_gate_perif_t* perif) {
    perif->timer_slice = pwm_gpio_to_slice_num(FREQUECY_PIN);

    gpio_set_function(FREQUECY_PIN, GPIO_FUNC_PWM);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv_mode(&config, PWM_DIV_B_RISING);
    pwm_config_set_clkdiv(&config, 1);
    pwm_init(perif->timer_slice, &config, false);
}

void sig_det_gate_timer_init(timer_gate_perif_t* perif) {
    perif->gate_slice = pwm_gpio_to_slice_num(0);

    pwm_set_clkdiv_int_frac(perif->gate_slice, GATE_TIMER_PRESCALER, 0);
    pwm_set_wrap(perif->gate_slice, GATE_TIMER_WRAP / 2 - 1);
    pwm_set_chan_level(perif->gate_slice, PWM_CHAN_B, GATE_TIMER_WRAP / 4);
    pwm_set_phase_correct(perif->gate_slice, true);

    perif->gate_dma_channel = dma_claim_unused_channel(true);
    dma_channel_config config =
        dma_channel_get_default_config(perif->gate_dma_channel);
    channel_config_set_transfer_data_size(&config, DMA_SIZE_32);
    channel_config_set_read_increment(&config, false);
    channel_config_set_dreq(&config, pwm_get_dreq(perif->gate_slice));
    io_rw_32* timer_slice_csr = &pwm_hw->slice[perif->timer_slice].csr;
    perif->csr_timer_stop = *timer_slice_csr;
    dma_channel_configure(perif->gate_dma_channel, &config,
                          &perif->csr_timer_stop, &perif->csr_timer_stop, 1,
                          false);
    pwm_set_enabled(perif->gate_slice, true);
}

bool sig_det_check_ready_gate(timer_gate_perif_t* perif) {
    if (dma_channel_is_busy(perif->gate_dma_channel)) {
        return false;
    } else {
        return true;
    }
}

void sig_det_counters_start(timer_gate_perif_t* perif) {
    dma_channel_transfer_from_buffer_now(perif->gate_dma_channel,
                                         &perif->csr_timer_stop, 1);
    pwm_set_counter(perif->gate_slice, 0);
    pwm_set_counter(perif->timer_slice, 0);
    pwm_set_mask_enabled((1 << perif->timer_slice) | (1 << perif->gate_slice));
}

void sig_det_get_freq_value(sig_det_t* perif) {
    while (dma_channel_is_busy(perif->gate_perif.gate_dma_channel))
        ;
    pwm_set_enabled(perif->gate_perif.gate_slice, false);
    perif->freq = pwm_get_counter(perif->gate_perif.timer_slice) *
                  SAMPLE_FREQ;  // TODO: upravit
}

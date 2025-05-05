#include "signal_detection.h"
#include "global_vars.h"
#include "hardware/dma.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

extern global_vars_t global_var;

void sig_det_frequecy_counter_init(timer_gate_perif_t* perif) {
    perif->timer_slice = pwm_gpio_to_slice_num(FREQUECY_PIN);
    gpio_init(FREQUECY_PIN);
    gpio_set_dir(FREQUECY_PIN, GPIO_IN);

    gpio_set_function(FREQUECY_PIN, GPIO_FUNC_PWM);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv_mode(&config, PWM_DIV_B_RISING);
    pwm_config_set_clkdiv(&config, 1);
    pwm_init(perif->timer_slice, &config, false);
}

void sig_det_pulse_detect_init(sig_det_t* det_perif) {
    uint32_t mask = (global_var.device_state == DEV_STATE_DETECT_PULSE_UP)
                        ? GPIO_IRQ_EDGE_RISE
                        : GPIO_IRQ_EDGE_FALL;

    gpio_set_irq_enabled_with_callback(FREQUECY_PIN, mask, true,
                                       &sig_det_pulse_detect_callback);
}

void sig_det_gate_timer_init(timer_gate_perif_t* perif) {
    perif->gate_slice = pwm_gpio_to_slice_num(0);

    pwm_set_clkdiv_int_frac(perif->gate_slice, GATE_TIMER_PRESCALER, 0);
    pwm_set_wrap(perif->gate_slice, GATE_WRAPS[perif->gate_index] / 2 - 1);
    pwm_set_chan_level(perif->gate_slice, PWM_CHAN_B,
                       GATE_WRAPS[perif->gate_index] / 4);
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

void sig_det_gate_timer_deinit(timer_gate_perif_t* perif) {
    pwm_set_enabled(perif->gate_slice, false);

    dma_channel_abort(perif->gate_dma_channel);

    dma_channel_unclaim(perif->gate_dma_channel);
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
                  (SAMPLE_FREQ / (GATE_TIMER_PRESCALER *
                                  GATE_WRAPS[perif->gate_perif.gate_index]));
}

void sig_det_pulse_detect_callback(uint gpio, uint32_t events) {
    if (gpio == FREQUECY_PIN) {
        if (events & GPIO_IRQ_EDGE_RISE &&
            global_var.device_state == DEV_STATE_DETECT_PULSE_UP) {
            global_var.sig_det_perif.pulse_found = true;
        } else if (events & GPIO_IRQ_EDGE_FALL &&
                   global_var.device_state == DEV_STATE_DETECT_PULSE_DOWN) {
            global_var.sig_det_perif.pulse_found = true;
        }
    }
}

void sig_det_calculate_rec(void) {
    sig_det_t* perif = &global_var.sig_det_perif;

    perif->widths[DET_HIGH_WIDTH] =
        perif->edge_times[DET_EDGE2_FALL] - perif->edge_times[DET_EDGE1_RISE];
    perif->widths[DET_LOW_WIDTH] =
        perif->edge_times[DET_EDGE3_RISE] - perif->edge_times[DET_EDGE2_FALL];
    perif->widths[DET_PERIOD_WIDTH] =
        perif->widths[DET_LOW_WIDTH] + perif->widths[DET_HIGH_WIDTH];
}

void __not_in_flash("gpio_callback")
    sig_det_recproc_pulse_callback(uint gpio, uint32_t events) {
    if (gpio != FREQUECY_PIN)
        return;
    sig_det_t* perif = &global_var.sig_det_perif;

    uint32_t current_time = timer_hw->timelr;

    if (events & GPIO_IRQ_EDGE_RISE) {
        if (perif->edge_mode == DET_EDGE1_RISE) {
            perif->edge_times[DET_EDGE1_RISE] = current_time;
            perif->edge_mode = DET_EDGE2_FALL;
            gpio_set_irq_enabled(FREQUECY_PIN, GPIO_IRQ_EDGE_RISE, false);
            gpio_set_irq_enabled(FREQUECY_PIN, GPIO_IRQ_EDGE_FALL, true);
        } else if (perif->edge_mode == DET_EDGE3_RISE) {
            perif->edge_times[DET_EDGE3_RISE] = current_time;
            perif->is_rec_finished = true;
            perif->is_rec = false;
            perif->edge_mode = DET_EDGE1_RISE;
            gpio_set_irq_enabled(
                FREQUECY_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false);
            sig_det_calculate_rec();
        }
    } else if (events & GPIO_IRQ_EDGE_FALL) {
        if (perif->edge_mode == DET_EDGE2_FALL) {
            perif->edge_times[DET_EDGE2_FALL] = current_time;
            perif->edge_mode = DET_EDGE3_RISE;
            // Přepnutí triggeru na RISING hranu
            gpio_set_irq_enabled(FREQUECY_PIN, GPIO_IRQ_EDGE_FALL, false);
            gpio_set_irq_enabled(FREQUECY_PIN, GPIO_IRQ_EDGE_RISE, true);
        }
    }
}

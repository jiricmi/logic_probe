#ifndef GPIO_OUTPUTS_H
#define GPIO_OUTPUTS_H

void gpio_init_timer(void);

void gpio_init_push_pull(void);

void gpio_deinit_pins(void);

void gpio_spi_master_init(void);

void gpio_spi_slave_init(void);
#endif

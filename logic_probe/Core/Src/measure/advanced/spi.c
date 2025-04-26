#include "advanced/spi.h"
#include <string.h>
#include "ansi_abstraction_layer.h"
#include "global_vars.h"
#include "gpio_outputs.h"
#include "ssd1306.h"
#include "stm32g0xx_hal_spi.h"

extern global_vars_t global_var;

void spi_init_struct(spi_perif_t* perif, SPI_HandleTypeDef* hspi) {
    memset(perif, 0, sizeof(*perif));
    perif->hspi = hspi;
    perif->bytes_count = 1;
}

void spi_deinit_perif(spi_perif_t* perif) {
    HAL_SPI_DMAStop(perif->hspi);
    HAL_SPI_DeInit(perif->hspi);
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0);
}

void spi_init_perif(spi_perif_t* perif) {
    perif->hspi->Instance = SPI1;
    perif->hspi->Init.Mode =
        (global_var.device_state == DEV_STATE_ADV_SPI_SLAVE) ? SPI_MODE_SLAVE
                                                             : SPI_MODE_MASTER;
    perif->hspi->Init.Direction = SPI_DIRECTION_2LINES;
    perif->hspi->Init.DataSize = SPI_DATASIZE_8BIT;
    perif->hspi->Init.CLKPolarity =
        (perif->CLK_polarity) ? SPI_POLARITY_HIGH : SPI_POLARITY_LOW;
    perif->hspi->Init.CLKPhase =
        (perif->CLK_phase) ? SPI_PHASE_2EDGE : SPI_PHASE_1EDGE;
    perif->hspi->Init.NSS = SPI_NSS_SOFT;
    perif->hspi->Init.FirstBit =
        (perif->msb) ? SPI_FIRSTBIT_LSB : SPI_FIRSTBIT_MSB;
    perif->hspi->Init.TIMode = SPI_TIMODE_DISABLE;
    perif->hspi->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    perif->hspi->Init.CRCPolynomial = 7;
    perif->hspi->Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
    perif->hspi->Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
    if (global_var.device_state != DEV_STATE_ADV_SPI_SLAVE) {
        perif->hspi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
    }
    if (HAL_SPI_Init(perif->hspi) != HAL_OK) {
        Error_Handler();
    }

    memset(perif->data, 0, SPI_ARRAY_SIZE);

    if (global_var.device_state == DEV_STATE_ADV_SPI_SLAVE) {
        gpio_spi_slave_init();
        HAL_SPI_Receive_DMA(perif->hspi, perif->data, perif->bytes_count);
    } else {
        gpio_spi_master_init();
    }
}

void spi_transmit(spi_perif_t* perif) {
    if (!perif->send_data) {
        return;
    }

    perif->send_data = 0;
    perif->error = SPI_ERROR_SUCCESS;
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
    if (!perif->read_bit) {
        if (HAL_SPI_Transmit(perif->hspi, perif->data, perif->bytes_count,
                             HAL_MAX_DELAY) != HAL_OK) {
            perif->error = SPI_ERROR_SEND;
        }
    } else {
        if (HAL_SPI_Transmit(perif->hspi, perif->master_send_data, 1,
                             HAL_MAX_DELAY) != HAL_OK) {
            perif->error = SPI_ERROR_SEND;
        }
        if (HAL_SPI_Receive(perif->hspi, perif->data, perif->bytes_count,
                            100) != HAL_OK) {
            perif->error = SPI_ERROR_RECEIVE;
        }
    }
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
}

void spi_test_display(spi_perif_t* perif) {
    if (perif->send_data) {
        perif->send_data = 0;
        SSD1306_spi_test_display(perif);
    }
}

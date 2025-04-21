#include "advanced/spi.h"
#include <string.h>
#include "stm32g0xx_hal_spi.h"

void spi_init_struct(spi_perif_t* perif, SPI_HandleTypeDef* hspi) {
    memset(perif, 0, sizeof(*perif));
    perif->hspi = hspi;
}

void spi_deinit_perif(spi_perif_t* perif) {
    HAL_SPI_DeInit(perif->hspi);
}

void spi_init_perif(spi_perif_t* perif) {
    perif->hspi->Instance = SPI1;
    perif->hspi->Init.Mode = SPI_MODE_MASTER;
    perif->hspi->Init.Direction = SPI_DIRECTION_2LINES;
    perif->hspi->Init.DataSize = SPI_DATASIZE_8BIT;
    perif->hspi->Init.CLKPolarity =
        (perif->CLK_polarity) ? SPI_POLARITY_HIGH : SPI_POLARITY_LOW;
    perif->hspi->Init.CLKPhase =
        (perif->CLK_phase) ? SPI_PHASE_2EDGE : SPI_PHASE_1EDGE;
    perif->hspi->Init.NSS = SPI_NSS_SOFT;
    perif->hspi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    perif->hspi->Init.FirstBit =
        (perif->msb) ? SPI_FIRSTBIT_MSB : SPI_FIRSTBIT_LSB;
    perif->hspi->Init.TIMode = SPI_TIMODE_DISABLE;
    perif->hspi->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    perif->hspi->Init.CRCPolynomial = 7;
    perif->hspi->Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
    perif->hspi->Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
    if (HAL_SPI_Init(perif->hspi) != HAL_OK) {
        Error_Handler();
    }
}

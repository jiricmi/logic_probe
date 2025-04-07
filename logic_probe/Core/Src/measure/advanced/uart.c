#include "advanced/uart.h"
#include <stdbool.h>
#include <string.h>

void uart_init(uart_perif_t* uart, UART_HandleTypeDef* huart) {
    memset(uart, 0, sizeof(*uart));
    uart->huart = huart;
    uart->word_len = UART_WORD_8B;
    uart->parity = UART_P_NONE;
    uart->stop_bits = UART_STOP_1;
    uart->baudrate = UART_DEFAULT_BAUDRATE;
    uart->edit = false;
}

void uart_start(uart_perif_t* uart) {
    uart->huart->Init.BaudRate = 115200;
    uart->huart->Init.WordLength = uart_get_word_len(uart);
    uart->huart->Init.StopBits = uart_get_stop_bit(uart);
    uart->huart->Init.Parity = uart_get_parity(uart);
    uart->huart->Init.Mode = UART_MODE_TX_RX;
    uart->huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    uart->huart->Init.OverSampling = UART_OVERSAMPLING_16;
    uart->huart->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    uart->huart->Init.ClockPrescaler = UART_PRESCALER_DIV1;
    uart->huart->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(uart->huart) != HAL_OK) {
        Error_Handler();
    }
}

uint32_t uart_get_word_len(uart_perif_t* uart) {
    switch (uart->word_len) {
        case UART_WORD_7B:
            return UART_WORDLENGTH_7B;
        case UART_WORD_8B:
            return UART_WORDLENGTH_8B;
        case UART_WORD_9B:
            return UART_WORDLENGTH_9B;
    }
    return UART_WORD_8B;
}

uint32_t uart_get_stop_bit(uart_perif_t* uart) {
    switch (uart->stop_bits) {
        case UART_STOP_1:
            return UART_STOPBITS_1;
        case UART_STOP_2:
            return UART_STOPBITS_2;
    }
    return UART_STOPBITS_1;
}

uint32_t uart_get_parity(uart_perif_t* uart) {
    switch (uart->parity) {
        case UART_P_NONE:
            return UART_PARITY_NONE;
        case UART_P_EVEN:
            return UART_PARITY_EVEN;
        case UART_P_ODD:
            return UART_PARITY_ODD;
    }
    return UART_PARITY_NONE;
}

void deinit_uart(uart_perif_t* uart) {
    HAL_UART_DeInit(uart->huart);  // TODO: HANDLE ERROR
}

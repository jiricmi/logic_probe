#ifndef UART_H
#define UART_H
#include <stdint.h>
#include "main.h"

#define UART_WORD_LEN_LEN 2
#define UART_PARITY_LEN 2
#define UART_STOP_BIT_LEN 1
#define UART_DEFAULT_BAUDRATE 115200;
#define UART_BUFFER_SIZE 50
#define UART_SEND_SIZE 10

typedef enum { UART_WORD_7B, UART_WORD_8B, UART_WORD_9B } uart_word_t;
typedef enum { UART_P_NONE, UART_P_EVEN, UART_P_ODD } uart_parity_t;
typedef enum { UART_STOP_1, UART_STOP_2 } uart_stopbit_t;

typedef struct {
    UART_HandleTypeDef* huart;
    uart_word_t word_len;
    unsigned char received_char[UART_BUFFER_SIZE];
    uart_parity_t parity;
    uart_stopbit_t stop_bits;
    uint32_t baudrate;
    uint8_t len;
    _Bool edit_send;
    _Bool edit;
    uint8_t edit_index;
    uint8_t symbols_to_send;
} uart_perif_t;

void uart_init(uart_perif_t* uart, UART_HandleTypeDef* huart);

void uart_start(uart_perif_t* uart);

void uart_send_bytes(uart_perif_t* uart);
void uart_start_receive(uart_perif_t* uart);

uint32_t uart_get_word_len(uart_perif_t* uart);
uint32_t uart_get_stop_bit(uart_perif_t* uart);

uint32_t uart_get_parity(uart_perif_t* uart);
void deinit_uart(uart_perif_t* uart);
#endif

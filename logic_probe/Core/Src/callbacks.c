#include "callbacks.h"

#include "ansi_abstraction_layer.h"
#include "ansi_pages.h"

char response[] = "Ahoj!\n";

extern char* current_page_bg_color;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {
    if (huart->Instance == USART2) {
        if (received_char == 'r') {  // reload
            ansi_main_page();
        } else if (received_char == 'k') {
            ansi_set_cursor(10, 10);
            ansi_send_text("Ahoj!", BLACK_TEXT, current_page_bg_color);
        } else if (received_char == 't') {
            ansi_logic_probe();
        }
        HAL_UART_Receive_IT(&huart2, &received_char, 1);
    }
}

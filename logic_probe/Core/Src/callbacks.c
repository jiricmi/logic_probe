#include "callbacks.h"
#include "ansi_abstraction_layer.h"
#include "ansi_pages.h"
#include "uart_control.h"

char response[] = "Ahoj!\n";

extern char* current_page_bg_color;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {
    if (huart->Instance == USART2) {
        if (received_char == 'r' || received_char == 'R') {  // reload
            render_current_page();
        } else {
            get_current_control();
        }

        HAL_UART_Receive_IT(&huart2, &received_char, 1);
    }
}

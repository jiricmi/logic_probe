#include "control_levels.h"
#include "ansi_abstraction_layer.h"
#include "ansi_pages.h"
#include "loop.h"

void control_levels_page(char received_char) {
    switch (received_char) {
        case 'q':
        case 'Q':
            ansi_clear_terminal();
            ansi_set_current_page(ANSI_PAGE_MAIN);
            dev_mode_change_mode(DEV_STATE_NONE);
            break;
        case '1':
            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
            break;
        case '2':
            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2);
            break;
        case '3':
            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_3);
            break;
    }
}

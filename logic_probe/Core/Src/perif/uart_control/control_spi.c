#include "control_spi.h"
#include "ansi_abstraction_layer.h"
#include "ansi_pages.h"
#include "loop.h"

void control_spi_page(char received_char) {
    switch (received_char) {
        case 'q':
        case 'Q':
            ansi_clear_terminal();
            ansi_set_current_page(ANSI_PAGE_MAIN_ADVANCED);
            dev_mode_change_mode(DEV_STATE_NONE);
            break;
    }
}

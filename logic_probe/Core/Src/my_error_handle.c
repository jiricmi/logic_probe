#include "my_error_handle.h"
#include "ansi_abstraction_layer.h"
#include "main.h"

void exception(const char* str) {
    while (1) {
        ansi_clear_terminal();
        ansi_text_config_t conf = {RED_TEXT, "", 1};
        ansi_send_text(str, &conf);
        HAL_Delay(1000);
    }
}

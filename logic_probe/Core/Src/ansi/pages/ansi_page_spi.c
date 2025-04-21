#include "ansi_page_spi.h"
#include <string.h>
#include "ansi_abstraction_layer.h"
#include "global_vars.h"

extern global_vars_t global_var;

void ansi_render_spi_measure_page(void) {
    ansi_render_border('%', "%", "");
    const char* header = "SCK - PA5 | MISO - PA6 | MOSI - PA7 | NSS - PB0";
    ansi_set_cursor(5, TERMINAL_CENTER - (strlen(header) / 2));
    ansi_send_text(header, &ansi_bold_conf);
}

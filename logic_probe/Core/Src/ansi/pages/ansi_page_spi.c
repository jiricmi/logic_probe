#include "ansi_page_spi.h"
#include <string.h>
#include "advanced/spi.h"
#include "ansi_abstraction_layer.h"
#include "global_vars.h"

extern global_vars_t global_var;
#define SPI_BUFFER_SIZE 60

#define SPI_EDIT_MESSAGE "Editing SPI settings!"

static void render_edit_spi_status(_Bool edit) {
    if (edit) {
        ansi_set_cursor(7, TERMINAL_CENTER - 10);
        ansi_send_text(SPI_EDIT_MESSAGE, &ansi_red_bold_conf);
    }
}

void ansi_render_spi_measure_page(void) {
    ansi_clear_terminal();
    ansi_render_border('%', "%", "");
    const char* header = "SCK-PA5/12 | MISO-PA6/13 | MOSI-PA7/14 | NSS-PB0/15";
    ansi_set_cursor(5, TERMINAL_CENTER - (strlen(header) / 2));
    ansi_send_text(header, &ansi_bold_conf);

    const uint8_t state = global_var.device_state;
    spi_perif_t* perif = global_var.spi_perif;

    if (perif == NULL)
        return;

    const char* title = "";
    switch (state) {
        case DEV_STATE_ADV_SPI_SLAVE:
            title = "SPI SLAVE";
            ansi_spi_render_slave_settings(perif);
            ansi_spi_render_read_vals(perif);
            help_slave_spi();
            break;

        case DEV_STATE_ADV_SPI_MASTER:
            title = "SPI MASTER";
            ansi_spi_render_master_settings(perif);
            ansi_spi_master_vals(perif);
            help_master_spi();
            break;

        case DEV_STATE_ADV_SPI_TEST_DISPLAY:
            title = "SPI TEST SSD1306";
            ansi_spi_render_master_settings(perif);
            ansi_spi_test_display_render_settings();
            help_spi_display();
            break;
    }

    if (*title != '\0') {
        ansi_set_cursor(4, TERMINAL_CENTER - (strlen(title) / 2));
        ansi_send_text(title, &ansi_bold_conf);
    }
    ansi_spi_print_error(perif);
}

void ansi_spi_test_display_render_settings(void) {
    const char* msg1 = "Press S to start checking display...";
    ansi_set_cursor(8, TERMINAL_CENTER - (strlen(msg1) / 2));
    ansi_send_text(msg1, &ansi_bold_conf);

    ansi_spi_print_error(global_var.spi_perif);
}

void ansi_spi_render_master_settings(spi_perif_t* perif) {
    char buff[SPI_BUFFER_SIZE];
    snprintf(buff, sizeof(buff),
             "Polarity: %d | Phase: %d | %s | %s | bytes to %s: %2d",
             perif->CLK_polarity, perif->CLK_phase,
             (perif->msb) ? "LSB" : "MSB", perif->read_bit ? "read" : "write",
             perif->read_bit ? "read" : "write", perif->bytes_count);
    ansi_set_cursor(6, TERMINAL_CENTER - (strlen(buff) / 2));
    ansi_send_text(buff, &ansi_default_conf);

    render_edit_spi_status(perif->edit_settings);
}

void ansi_spi_render_slave_settings(spi_perif_t* perif) {
    char buff[SPI_BUFFER_SIZE];
    snprintf(buff, sizeof(buff),
             "Polarity: %d | Phase: %d | %s | bytes to read: %2d",
             perif->CLK_polarity, perif->CLK_phase,
             (perif->msb) ? "LSB" : "MSB", perif->bytes_count);
    ansi_set_cursor(6, TERMINAL_CENTER - (strlen(buff) / 2));
    ansi_send_text(buff, &ansi_default_conf);

    render_edit_spi_status(perif->edit_settings);
}

void ansi_spi_render_read_vals(spi_perif_t* perif) {
    char buff[6];
    ansi_set_cursor(12, TERMINAL_CENTER - (2 * perif->bytes_count));
    for (uint8_t i = 0; i < perif->bytes_count; ++i) {
        snprintf(buff, sizeof(buff), "0x%02X ", perif->data[i]);
        ansi_send_text(buff, &ansi_default_conf);
    }
}

void ansi_spi_master_vals(spi_perif_t* perif) {
    char buff[6];
    const uint8_t base_row = 8;
    const uint8_t base_col = TERMINAL_CENTER;

    if (perif->read_bit) {
        snprintf(buff, sizeof(buff), "0x%02X", perif->master_send_data[0]);
        ansi_set_cursor(base_row, base_col - (strlen(buff) / 2));
        ansi_send_text(buff, perif->edit_vals ? &ansi_green_bold_conf
                                              : &ansi_default_conf);

        ansi_set_cursor(10, base_col - 5 - (3 * perif->bytes_count));
        for (uint8_t i = 0; i < perif->bytes_count; ++i) {
            ansi_send_text(" | ", &ansi_default_conf);
            snprintf(buff, sizeof(buff), "0x%02X", perif->data[i]);
            ansi_send_text(buff, &ansi_default_conf);
        }
        ansi_send_text(" |", &ansi_default_conf);

    } else {
        ansi_set_cursor(10, base_col - 5 - (3 * perif->bytes_count));
        for (uint8_t i = 0; i < perif->bytes_count; ++i) {
            ansi_send_text(" | ", &ansi_default_conf);
            snprintf(buff, sizeof(buff), "0x%02X", perif->data[i]);
            ansi_send_text(buff, (perif->edit_vals && i == perif->master_index)
                                     ? &ansi_green_bold_conf
                                     : &ansi_default_conf);
        }
        ansi_send_text(" |", &ansi_default_conf);
    }
}

void ansi_spi_print_error(spi_perif_t* perif) {
    ansi_clear_line(15, 1);
    ansi_set_cursor(15, TERMINAL_CENTER - 10);
    switch (perif->error) {
        case SPI_ERROR_SUCCESS:
            ansi_send_text("SPI OK", &ansi_default_conf);
            break;
        case SPI_ERROR_RECEIVE:
            ansi_send_text("SPI RECEIVE ERROR", &ansi_red_bold_conf);
            break;
        case SPI_ERROR_SEND:
            ansi_send_text("SPI SEND ERROR", &ansi_red_bold_conf);
            break;
        default:
            break;
    }
}

void help_slave_spi(void) {
    if (global_var.spi_perif->edit_settings) {
        ansi_print_help_msg("T: stop edit | U: Polarity | Y: Phase ", 1);
        ansi_print_help_msg("I: byte count | O: MSB/LSB", 0);
    } else {
        ansi_print_help_msg(
            "T: edit settings | M: change mode | G: reset perif", 0);
    }
}

void help_master_spi(void) {
    if (global_var.spi_perif->edit_settings) {
        ansi_print_help_msg("T: stop edit | U: Polarity | Y: Phase ", 1);
        ansi_print_help_msg("I: byte count | O: MSB/LSB | P: read/write", 0);
    } else if (global_var.spi_perif->edit_vals) {
        ansi_print_help_msg(
            "K: stop edit | L: move cursor | 0-F: edit val | X: delete val", 0);

    } else {
        ansi_print_help_msg(
            "S: send | T: edit settings | K: edit vals | M: change mode | G: "
            "reset perif",
            0);
    }
}

void help_spi_display(void) {
    if (global_var.spi_perif->edit_settings) {
        ansi_print_help_msg("T: stop edit | U: Polarity | Y: Phase ", 1);
        ansi_print_help_msg("I: byte count | O: MSB/LSB | P: read/write", 0);
    } else {
        ansi_print_help_msg("S: send | T: edit settings | M: change mode | G: reset perif", 0);
    }
}

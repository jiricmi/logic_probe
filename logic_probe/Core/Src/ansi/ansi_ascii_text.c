#include "ansi_ascii_text.h"
#include <stdlib.h>
#include <string.h>

ascii_text* get_ascii_struct(const ascii_logo_type_t ascii) {
    ascii_text* ascii_struct;
    switch (ascii) {
        case ASCII_LOGO_MAIN:
            ascii_struct = allocate_ascii_struct(MAIN_ASCII_TITLE_ROW,
                                                 MAIN_ASCII_TITLE_COL);
            const char* main_ascii[] = MAIN_ASCII_TITLE;
            fill_ascii_struct(ascii_struct, main_ascii);
            break;
        case ASCII_LOGO_CHANNEL:
            ascii_struct = allocate_ascii_struct(CHANNEL_ASCII_TITLE_ROW,
                                                 CHANNEL_ASCII_TITLE_COL);
            const char* channel_ascii[] = CHANNEL_ASCII_TITLE;
            fill_ascii_struct(ascii_struct, channel_ascii);
            break;
        case ASCII_LOGO_VOLTAGE:
            ascii_struct = allocate_ascii_struct(VOLTAGE_ASCII_TITLE_ROW,
                                                 VOLTAGE_ASCII_TITLE_COL);
            const char* voltage_ascii[] = VOLTAGE_ASCII_TITLE;
            fill_ascii_struct(ascii_struct, voltage_ascii);
            break;
        case ASCII_LOGO_SIGNAL_DETECT:
            ascii_struct = allocate_ascii_struct(SIGNAL_DETECT_ASCII_TITLE_ROW,
                                                 SIGNAL_DETECT_ASCII_TITLE_COL);
            const char* signal_detect_ascii[] = SIGNAL_DETECT_ASCII_TITLE;
            fill_ascii_struct(ascii_struct, signal_detect_ascii);
            break;
        case ASCII_LOGO_SIGNAL_GENERATOR:
            ascii_struct =
                allocate_ascii_struct(SIGNAL_GENERATOR_ASCII_TITLE_ROW,
                                      SIGNAL_GENERATOR_ASCII_TITLE_COL);
            const char* signal_generator_ascii[] = SIGNAL_GENERATOR_ASCII_TITLE;
            fill_ascii_struct(ascii_struct, signal_generator_ascii);
            break;

        default:
            return NULL;
    }

    return ascii_struct;
}

ascii_text* allocate_ascii_struct(const size_t row, const size_t col) {
    ascii_text* ascii_struct = (ascii_text*)malloc(sizeof(ascii_text));

    if (ascii_struct == NULL) {
        // todo: neco udelej
    }

    ascii_struct->row = row;
    ascii_struct->col = col;

    ascii_struct->ascii = (char**)malloc(sizeof(char*) * row);

    if (ascii_struct->ascii == NULL) {
        // todo: neco udelej
    }

    for (size_t i = 0; i < row; ++i) {
        ascii_struct->ascii[i] = (char*)malloc(sizeof(char) * (col + 1));
        if (ascii_struct->ascii[i] == NULL) {
            // todo: neco udelej
        }
    }
    return ascii_struct;
}

void fill_ascii_struct(const ascii_text* ascii_struct, const char** ascii) {
    for (size_t i = 0; i < ascii_struct->row; ++i) {
        strncpy(ascii_struct->ascii[i], ascii[i], ascii_struct->col);
        ascii_struct->ascii[i][ascii_struct->col] = '\0';
    }
}

void free_ascii_text(ascii_text* ascii_struct) {
    for (size_t i = 0; i < ascii_struct->row; ++i) {
        free(ascii_struct->ascii[i]);
    }

    free(ascii_struct->ascii);
    free(ascii_struct);
}

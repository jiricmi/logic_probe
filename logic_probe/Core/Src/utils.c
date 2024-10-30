#include "utils.h"
#include <stdint.h>

unsigned int join_strings(char* buffer, char* join, unsigned int start) {
    int i = 0;
    while (join[i] != '\0') {
        buffer[start++] = join[i++];
    }
    return start;
}

void flip_buffer(char* buffer, const int size) {
    for (int i = 0, j = size - 1; i < j; i++, j--) {
        char temp = buffer[i];
        buffer[i] = buffer[j];
        buffer[j] = temp;
    }
}

void int_to_string(char* buffer, int number) {
    int it = 0;
    uint8_t negative = 0;

    if (number == 0) {
        buffer[it++] = '0';
        buffer[it] = '\0';
        return;
    }

    if (number < 0) {
        number = -number;
        negative = 1;
    }

    while (number != 0) {
        buffer[it++] = (number % 10) + '0';
        number /= 10;
    }

    if (negative) {
        buffer[it++] = '-';
    }

    flip_buffer(buffer, it);

    buffer[it] = '\0';
}

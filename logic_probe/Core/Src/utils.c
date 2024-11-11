#include "utils.h"
#include <stdint.h>
#include <string.h>

unsigned int join_strings(char* buffer, char* join, unsigned int start) {
    int i = 0;
    while (join[i] != '\0') {
        buffer[start++] = join[i++];
    }
    return start;
}

void uint_32_to_split_int(unsigned int* buff,
                          uint32_t n,
                          unsigned int floating_point) {
    if (floating_point == 0) {
        buff[0] = n;
        buff[1] = 0;
        return;
    }

    unsigned int decimal = n / (unsigned int)positive_power_10(floating_point);
    unsigned int floating = n % (unsigned int)positive_power_10(floating_point);

    buff[0] = decimal;
    buff[1] = floating;
}

void clear_buffer(void* buffer, size_t size) {
    memset(buffer, 0, size);
}

unsigned int positive_power_10(unsigned int power) {
    unsigned int base = 1;
    for (size_t i = 0; i < power; ++i) {
        base *= 10;
    }

    return base;
}

// char digit to int
int cdtoi(char digit) {
    if (digit >= '0' && digit <= '9') {
        return digit - '0';
    } else {
        return -1;
    }
}

char itocd(int digit) {
    if (digit >= 0 && digit <= 9) {
        return (char)digit + '0';
    } else {
        // todo: handle error
        return '0';
    }
}

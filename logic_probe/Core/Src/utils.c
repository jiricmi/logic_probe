#include "utils.h"
#include <stdint.h>
#include <string.h>

uint32_t join_strings(char* buffer, char* join, uint32_t start) {
    int i = 0;
    while (join[i] != '\0') {
        buffer[start++] = join[i++];
    }
    return start;
}

void uint_32_to_split_int(uint32_t* buff, uint32_t n, uint32_t floating_point) {
    if (floating_point == 0) {
        buff[0] = n;
        buff[1] = 0;
        return;
    }

    uint32_t decimal = n / positive_power_10(floating_point);
    uint32_t floating = n % positive_power_10(floating_point);

    buff[0] = decimal;
    buff[1] = floating;
}

int32_t utils_round(int32_t n) {
    return (n + 5) / 10;
}

void clear_buffer(void* buffer, size_t size) {
    memset(buffer, 0, size);
}

uint32_t positive_power_10(uint32_t power) {  // TODO: REFACTOR
    unsigned int base = 1;
    for (size_t i = 0; i < power; ++i) {
        base *= 10;
    }

    return base;
}

uint32_t positive_power(const uint32_t power, const uint32_t of) {
    unsigned int base = 1;
    for (size_t i = 0; i < power; ++i) {
        base *= of;
    }

    return base;
}

// char digit to int
int32_t cdtoi(char digit) {
    if (digit >= '0' && digit <= '9') {
        return digit - '0';
    } else {
        return -1;
    }
}

char itocd(int32_t digit) {
    if (digit >= 0 && digit <= 9) {
        return (char)digit + '0';
    } else {
        // todo: handle error
        return '0';
    }
}

int32_t digits_count(int32_t n) {
    int32_t count = 0;
    if (n < 0) {
        n = -n;
    }

    do {
        ++count;
        n /= 10;
    } while (n != 0);

    return count;
}

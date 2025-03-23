#ifndef UTILS_H
#define UTILS_H
#include <stdint.h>
#include "stdio.h"

uint32_t join_strings(char* buffer, char* join, uint32_t start);

void uint_32_to_split_int(uint32_t* buff, uint32_t n, uint32_t floating_point);

int32_t utils_round(int32_t n);

void clear_buffer(void* buffer, size_t size);

uint32_t positive_power_10(uint32_t power);

uint32_t positive_power(const uint32_t power, const uint32_t of);

int32_t cdtoi(char digit);

char itocd(int32_t digit);

int32_t digits_count(int32_t n);
#endif

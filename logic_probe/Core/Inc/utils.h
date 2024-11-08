#ifndef UTILS_H
#define UTILS_H
#include <stdint.h>
#include "stdio.h"

unsigned int join_strings(char* buffer, char* join, unsigned int start);

void uint_32_to_split_int(unsigned int* buff,
                          uint32_t n,
                          unsigned int floating_point);

void clear_buffer(void* buffer, size_t size);
#endif

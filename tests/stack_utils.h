#ifndef P256_STACK_UTILS_H
#define P256_STACK_UTILS_H
#include <stdint.h>

// Extremely rudimentary, very brittle, you probably don't want to use this file.

void stack_fill(size_t len);
size_t stack_find_marker(size_t size_max);

#endif //P256_STACK_UTILS_H

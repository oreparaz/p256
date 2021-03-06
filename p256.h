#pragma once

typedef enum {
  P256_SUCCESS = 1,
  P256_INVALID_SIGNATURE = 2
} p256_ret_t;


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

p256_ret_t p256_verify(uint8_t *msg, size_t msg_len, uint8_t *sig, const uint8_t *pk);

#ifdef __cplusplus
}
#endif

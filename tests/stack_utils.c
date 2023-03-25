#include <stdint.h>
#include <string.h>

#include "stack_utils.h"

// Extremely rudimentary, very brittle, you probably don't want to use this file.
//
// Among other big no-nos, this file relies on undefined behavior.

static uint64_t magic = 0xBADC0DECAFEBABE;
#define BUF_LEN (512)

_Static_assert(sizeof(void*) == 8 || sizeof(void*) == 4, "incorrect assumption");

#define IS_64 ((sizeof(void*)==8))
#define ptr_size ((IS_64) ? 8 : 4)

static uint8_t condition(uint64_t addr, uint8_t *mem) {
    uint8_t ret = 1;
    ret &= (mem[0] ==  (addr&0x00000000000000FF));
    ret &= (mem[1] == ((addr&0x000000000000FF00)>>8));
    ret &= (mem[2] == ((addr&0x0000000000FF0000)>>16));
    ret &= (mem[3] == ((addr&0x00000000FF000000)>>24));
    if (IS_64) {
        ret &= (mem[4] == ((addr&0x000000FF00000000)>>32));
        ret &= (mem[5] == ((addr&0x0000FF0000000000)>>40));
        ret &= (mem[6] == ((addr&0x00FF000000000000)>>48));
        ret &= (mem[7] == ((addr&0xFF00000000000000)>>56));
    }

    return ret;
}

size_t stack_find_marker(size_t size_max) {
    uint8_t x = 42;
    uint8_t *ptr = &x;
    for (size_t i=0; i<size_max; i++) {
        uint8_t *mem = ptr - i; // this is undefined behavior. yolo.
        uint8_t ret = 1;

        for (size_t j = 0; j<(BUF_LEN/ptr_size); j++) {
            uint64_t addr = (uint64_t)&mem[0];
            addr ^= magic;
            ret &= condition(addr, mem);
            mem -= ptr_size;
        }

        if (ret==1) return i;
    }
    return 0;
}

// will fill at least len bytes, but likely more
void stack_fill(size_t len)
{
    volatile uint8_t buf[BUF_LEN];

    for (size_t i=0; i<(sizeof(buf)*sizeof(buf[0])/ptr_size); i++) {
        size_t off = i*ptr_size;
        uint64_t addr = (uint64_t)&buf[off];
        uint64_t val = addr ^ magic;
        buf[off+0] =  val&0x000000000000000FF;
        buf[off+1] = (val&0x000000000000FF00) >> 8;
        buf[off+2] = (val&0x0000000000FF0000) >> 16;
        buf[off+3] = (val&0x00000000FF000000) >> 24;
        if (IS_64) {
            buf[off+4] = (val&0x000000FF00000000) >> 32;
            buf[off+5] = (val&0x0000FF0000000000) >> 40;
            buf[off+6] = (val&0x00FF000000000000) >> 48;
            buf[off+7] = (val&0xFF00000000000000) >> 56;
        }
    }

    if (len > sizeof(buf)) {
        stack_fill(len - sizeof(buf)*sizeof(buf[0]));
    }
}

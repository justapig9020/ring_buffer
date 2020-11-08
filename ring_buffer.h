#ifndef _RING_BUFFER_H_
#define _RING_BUFFER_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

enum {
    RB_ALLOW_SLICE,
    RB_ENTIRE,
    RB_MODE_AMOUNT,
};

struct Ring_buffer {
    uint8_t *buffer;
    size_t size;
    size_t base;
    size_t content;
    int (*read)(struct Ring_buffer *rb, void *dbuf, size_t len);
    int (*write)(struct Ring_buffer *rb, const void *sbuf, size_t len);
};

bool init_rbuf(struct Ring_buffer *rb, uint8_t *buf, size_t size, int mode);

#endif

#include <stdint.h>
#include "ring_buffer.h"

static int slice_read(struct Ring_buffer *rb, void *dbuf, size_t len) {
    return -1;
}

static int slice_write(struct Ring_buffer *rb, const void *dbuf, size_t len) {
    return -1;
}

static int entire_read(struct Ring_buffer *rb, void *dbuf, size_t len) {
    size_t readable = rb->content;
    if (readable < len)
        return -1;
    return 0;
}

static int entire_write(struct Ring_buffer *rb, const void *dbuf, size_t len) {
    size_t writable = rb->size - rb->content;
    if (writable < len)
        return -1;
    return 0;
}

bool init_rbuf(struct Ring_buffer *rb, uint8_t *buf, size_t size, int mode) {
    rb->buffer = buf;
    rb->size = size;
    rb->base = rb->content = 0;
    switch(mode) {
        case RB_ALLOW_SLICE:
            rb->read = slice_read;
            rb->write = slice_write;
            break;
        case RB_ENTIRE:
            rb->read = entire_read;
            rb->write = entire_write;
            break;
        default:
            return false;
    }
    return true;
}

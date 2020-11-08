#include <string.h>
#include <stdint.h>
#include "ring_buffer.h"

static int slice_read(struct Ring_buffer *rb, void *dbuf, size_t len) {
    uint8_t *src = rb->buffer;
    uint8_t *des = dbuf;
    size_t read_len = len > rb->content? rb->content : len;
    size_t len1 = 0;
    if (rb->base + read_len >= rb->size) {
        len1 = rb->size - rb->base;
        memcpy(des, &src[rb->base], len1);
        rb->base = 0;
    }
    size_t len2 = read_len - len1;
    memcpy(&des[len1], &src[rb->base], len2);
    rb->base += len2;
    rb->content -= read_len;
    return  read_len;
}

static int slice_write(struct Ring_buffer *rb, const void *sbuf, size_t len) {
    const uint8_t *src = sbuf;
    uint8_t *des = rb->buffer;
    size_t rest = rb->size - rb->content;
    size_t write_len = len > rest? rest : len;
    size_t base = rb->base + rb->content >= rb->size?
        rb->content + rb->base - rb->size : rb->base + rb->content;
    size_t len1 = 0;
    if (base + write_len >= rb->size) {
        len1 = rb->size - base;
        memcpy(&des[base], src, len1);
        base = 0;
    }
    size_t len2 = write_len - len1;
    memcpy(&des[base], &src[len1], len2);
    rb->content += write_len;
    return write_len;
}

static int entire_read(struct Ring_buffer *rb, void *dbuf, size_t len) {
    size_t readable = rb->content;
    if (readable < len)
        return -1;
    return slice_read(rb, dbuf, len);
}

static int entire_write(struct Ring_buffer *rb, const void *sbuf, size_t len) {
    size_t writable = rb->size - rb->content;
    if (writable < len)
        return -1;
    return slice_write(rb, sbuf, len);
}

bool init_rbuf(struct Ring_buffer *rb, uint8_t *buf, size_t size, int mode) {
    rb->buffer = buf;
    rb->size = size;
    rb->base = rb->content = 0;
    switch (mode) {
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

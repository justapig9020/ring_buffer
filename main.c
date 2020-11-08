#include <stdio.h>
#include <stdint.h>
#include "ring_buffer.h"
#define ARR_SZ(arr) (sizeof(arr) / sizeof(arr[0]))

int main(void) {
    uint8_t buf[100];
    size_t size = ARR_SZ(buf);
    struct Ring_buffer rb;
    if (!init_rbuf(&rb, buf, size, RB_ALLOW_SLICE)) {
        printf("Err\n");
        return 1;
    }
    for (int i=0; i<100; i++) {
        char c = 'a' + (i % 26);
        rb.write(&rb, &c, 1);
    }
    uint8_t read_back[100];
    printf("Read: %d\n", rb.read(&rb, read_back, 100));
    for (int i=0; i<100; i++) {
        if (i % 26 == 0)
            printf("\n");
        printf("%c", read_back[i]);
    }
    printf("\n");

    for (int i=0; i<50; i++) {
        rb.write(&rb, "a", 1);
    }
    printf("Read: %d\n", rb.read(&rb, read_back, 30));
    for (int i=0; i<80; i++) {
        rb.write(&rb, "b", 1);
    }

    printf("Read: %d\n", rb.read(&rb, read_back, 100));
    for (int i=0; i<100; i++)
        printf("%c", read_back[i]);
    printf("\n");
    printf("Read: %d\n", rb.read(&rb, read_back, 100));
    return 0;
}

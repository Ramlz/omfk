#include "common/common.h"
#include "lib/string.h"

void memset(void *ptr, const uint8_t value, uint32_t size) {
    while (size--) {
        *((uint8_t*) ptr++) = value;
    }
}

void memcpy(void *dest, void *src, uint32_t n) {
    char *csrc = (char *)src;
    char *cdest = (char *)dest;

    for (uint32_t i = 0; i < n; i++) {
        cdest[i] = csrc[i];
    }
}

int memcmp(const void *ptr1, const void *ptr2, uint32_t n) {
    unsigned char size1, size2;

    for (; n--; ptr1++, ptr2++) {
        size1 = *(unsigned char*) ptr1;
        size2 = *(unsigned char*) ptr2;
        if (size1 != size2) {
            return (size1 - size2);
        }
    }
    return 0;
}

void error_message(const char *message) {
    printf("[AUHTUNG] %s\r", message);
}

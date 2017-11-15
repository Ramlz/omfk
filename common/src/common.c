#include "common.h"

void memset(void *ptr, const uint8_t value, uint32_t size) {
    while (size--) {
        *((uint8_t*) ptr++) = value;
    }
}

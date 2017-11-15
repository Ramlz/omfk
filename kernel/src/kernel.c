#include "kernel.h"

void kernel(void) {
    clock_init();
    systick_init();
    heap_init();
    timers_init();
    dht_init();
    terminal_init();
    peons_init();
}

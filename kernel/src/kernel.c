#include "kernel.h"

void kernel(void) {
    clock_init();
    systick_init();
    heap_init();
    terminal_init();
    dht_init();
    terminal_info_message("Init completed!");
    terminal_start();
}

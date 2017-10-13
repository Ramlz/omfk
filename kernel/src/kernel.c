#include "kernel.h"

void kernel(void) {
    init_clock();
    init_systick();
    heap_init();
    terminal_init();
    terminal_info_message("Init completed!");
    terminal_start();
}

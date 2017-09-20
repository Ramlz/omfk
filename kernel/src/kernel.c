#include "kernel.h"

void kernel(void) {
    init_clock();
    init_systick();
    init_gpio();
    terminal_init();
    // init_dac();
    // init_dma();
    // init_timers();
    terminal_info_message("Init completed!");
    terminal_start();
}

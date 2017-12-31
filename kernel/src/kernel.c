#include "kernel.h"

void kernel(void) {
    clock_init();
    systick_init();
    heap_init();
    dht_init();
    led_init();
    terminal_init();
    esp_init();
    peons_init();
    {
        // create terminal thread
        peon_create(TASK_PTR(terminal_start));
        // create dht thread
        peon_create(TASK_PTR(dht_task));
        // create led blink thread
        peon_create(TASK_PTR(led_loop));
    }
    // enter the user mode
    user_mode();
}

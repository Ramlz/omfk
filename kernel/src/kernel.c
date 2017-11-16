#include "kernel.h"

void peon_dht(void) {
    while (1) {
        context_lock();
        {
            // read DHT sensor data
            dht_read();
        }
        context_unlock();

        clock_dly_secs(1);
    }
}

void kernel(void) {
    clock_init();
    systick_init();
    heap_init();
    timers_init();
    dht_init();
    terminal_init();
    peons_init();
    {
        // create terminal thread
        peon_create(TASK_PTR(terminal_start));
        // create dht thread
        peon_create(TASK_PTR(peon_dht));
    }
    // enter the user mode
    user_mode();
}

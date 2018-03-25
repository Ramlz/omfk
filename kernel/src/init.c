#include "arch/context.h"
#include "arch/nvic.h"
#include "arch/systick.h"
#include "platform/clock.h"
#include "kernel/init.h"
#include "kernel/memory.h"
#include "kernel/peons.h"
#include "peons/led.h"
#include "peons/log.h"
#include "peons/sensors.h"
#include "peons/terminal.h"

/**
 * @brief      basic threads initialization and user mode switch
 */
static void init_peons(void) {
    peons_init();
    //! create terminal thread
    peon_create(TASK_PTR(terminal_loop), "terminal");
    //! create dht thread
    peon_create(TASK_PTR(sensors_loop), "sensors");
    //! create led blink thread
    peon_create(TASK_PTR(led_loop), "led");
    //! create log monitor thread
    peon_create(TASK_PTR(log_loop), "log");
    //! enter the user mode
    sv_call(SVC_USER_MODE);
}

void init_subsystems(void) {
    systick_init();
    clock_init();
    heap_init();
    gpio_init();
    terminal_init();

    init_peons();
}

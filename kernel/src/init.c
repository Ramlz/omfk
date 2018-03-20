#include "init.h"
#include "terminal.h"
#include "clock.h"
#include "memory.h"
#include "dht.h"
#include "context.h"
#include "led.h"
#include "log.h"
#include "pcd8544.h"
#include "nvic.h"
#include "peons.h"

/**
 * lcd driver configuration
 */
static const pcd8544_config lcd_config = {
    .port                    = GPIOB,

    .rst_pin                 = 11,
    .ce_pin                  = 12,
    .dc_pin                  = 13,
    .din_pin                 = 14,
    .clk_pin                 = 15,

    .contrast                = 60,
    .mux_rate                = PCD8544_MUX_RATE_FORTY,
    .display_mode            = PCD8544_DISPLAY_MODE_NORMAL
};

/**
 * @brief      basic threads initialization and user mode switch
 */
static void init_peons(void) {
    peons_init();
    //! create terminal thread
    peon_create(TASK_PTR(terminal_start), "terminal");
    //! create dht thread
    peon_create(TASK_PTR(dht_task), "dht");
    //! create led blink thread
    peon_create(TASK_PTR(led_loop), "led");
    //! create log monitor thread
    peon_create(TASK_PTR(log_task), "log");
    //! enter the user mode
    sv_call(SVC_USER_MODE);
}

void init_subsystems(void) {
    clock_init();
    systick_init();
    heap_init();
    gpio_init();
    dht_init();
    led_init();
    terminal_init();
    pcd8544_init(&lcd_config);

    init_peons();
}

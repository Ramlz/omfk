#include "arch/context.h"
#include "arch/nvic.h"
#include "arch/systick.h"
#include "platform/clock.h"
#include "platform/timer.h"
#include "kernel/init.h"
#include "kernel/memory.h"
#include "kernel/peons.h"
#include "driver/pcd8544.h"
#include "peons/led.h"
#include "peons/log.h"
#include "peons/sensors.h"
#include "peons/terminal.h"

/**
 * lcd driver configuration
 */
static const pcd8544_config lcd_config = {
    .port                    = PCD8544_PORT,

    .rst_pin                 = PCD8544_PIN_RST,
    .ce_pin                  = PCD8544_PIN_CE,
    .dc_pin                  = PCD8544_PIN_DC,
    .din_pin                 = PCD8544_PIN_DIN,
    .clk_pin                 = PCD8544_PIN_CLK,

    .contrast                = 60,
    .mux_rate                = PCD8544_MUX_RATE_FORTY,
    .display_mode            = PCD8544_DISPLAY_MODE_NORMAL
};

static const timer_config tim1_config = {
    .counter_mode = TIMER_COUNTER_MODE_UP,
    .prescaler    = 0x3f,
    .period       = 0xff
};

/**
 * @brief      basic threads initialization and user mode switch
 */
static void init_peons(void) {
    peons_init();
    //! create terminal thread
    peon_create(TASK_PTR(terminal_start), "terminal");
    //! create dht thread
    peon_create(TASK_PTR(sensor_loop), "sensor");
    //! create led blink thread
    peon_create(TASK_PTR(led_loop), "led");
    //! create log monitor thread
    peon_create(TASK_PTR(log_task), "log");
    //! enter the user mode
    sv_call(SVC_USER_MODE);
}

static void init_timers() {
    timer_init(TIM1, &tim1_config);
    timer_init(TIM2, NULL);
    timer_init(TIM3, NULL);
    timer_init(TIM4, NULL);
    timer_init(TIM6, NULL);
    timer_init(TIM7, NULL);
    timer_init(TIM8, NULL);
    timer_init(TIM15, NULL);
    timer_init(TIM16, NULL);
    timer_init(TIM17, NULL);
    timer_init(TIM20, NULL);
}

void init_subsystems(void) {
    systick_init();
    clock_init();
    heap_init();
    gpio_init();
    init_timers();
    pcd8544_init(&lcd_config);
    terminal_init();

    init_peons();
}

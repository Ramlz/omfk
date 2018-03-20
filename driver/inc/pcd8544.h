#ifndef PCD8544_H
#define PCD8544_H

#include "gpio.h"
#include "common.h"

/**
 * lcd mux rates
 */
typedef enum pcd8544_mux_rate_t {
    PCD8544_MUX_RATE_HUNDRED              = 0x10,
    PCD8544_MUX_RATE_EIGHTY               = 0x11,
    PCD8544_MUX_RATE_SIXTY_FIVE           = 0x12,
    PCD8544_MUX_RATE_FORTY_EIGHT          = 0x13,
    PCD8544_MUX_RATE_FORTY                = 0x14,
    PCD8544_MUX_RATE_TWENTY_FOUR          = 0x15,
    PCD8544_MUX_RATE_EIGHTEEN             = 0x16,
    PCD8544_MUX_RATE_TEN                  = 0x17
} __attribute__((packed,aligned(1))) pcd8544_mux_rate;

/**
 * lcd display modes
 */
typedef enum pcd8544_display_mode_t {
    PCD8544_DISPLAY_MODE_BLANK            = 0x8,
    PCD8544_DISPLAY_MODE_NORMAL           = 0xc,
    PCD8544_DISPLAY_MODE_ALL_SEGMENTS     = 0x9,
    PCD8544_DISPLAY_MODE_INVERSE          = 0xd
} __attribute__((packed,aligned(1))) pcd8544_display_mode;

/**
 * lcd driver config
 *
 * NOTE: all pins should be inside one GPIO port
 */
typedef struct pcd8544_config_t {
    gpio_port              port;

    uint32_t               rst_pin;
    uint32_t               ce_pin;
    uint32_t               dc_pin;
    uint32_t               din_pin;
    uint32_t               clk_pin;

    int8_t                 contrast;
    pcd8544_mux_rate       mux_rate;
    pcd8544_display_mode   display_mode;
} pcd8544_config;

/**
 * lcd driver interface
 */
typedef struct pcd8544_iface_t {
    /**
     * @brief      draw image on lcd screen
     *
     * @param      iface       lcd driver interface
     * @param[in]  buffer      image buffer
     * @param[in]  buffer_len  image buffer length
     */
    void (*draw)(struct pcd8544_iface_t *iface, const uint8_t *buffer,
        uint32_t buffer_len);
    /**
     * @brief      fill screen with value
     *
     * @param      iface  lcd driver interface
     * @param[in]  fill   value to fill
     */
    void (*clear)(struct pcd8544_iface_t *iface, bool fill);
    /**
     * @brief      deallocate lcd driver context
     *
     * @param      iface  lcd driver interface
     */
    void (*destroy)(struct pcd8544_iface_t *iface);
} pcd8544_iface;

/**
 * @brief      get driver interface
 *
 * @return     interface ptr
 */
pcd8544_iface *pcd8544_iface_get(void);

/**
 * @brief      initialize lcd by given config
 *
 * @param[in]  config  driver configuration
 */
void pcd8544_init(const pcd8544_config *config);

#endif

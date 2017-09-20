#ifndef DAC_H
#define DAC_H

#include "stm32f303xx.h"

#define SAMPLES_PER_WAVE 0x100
/**
 * @brief      Initialize DAC channels and turn on it's clock
 */
void init_dac(void);
/**
 * @brief      Writes 8 bit value to DAC1 channel 1 data register
 *
 * @param[in]  value  The 8 bit value
 */
void write_dac_8RA(uint8_t value);
/**
 * @brief      Gets the sine wave array address
 *
 * @return     The address
 */
uint32_t get_sine_wave(void);

#endif

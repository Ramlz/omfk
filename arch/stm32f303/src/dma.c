#include "dma.h"

void init_dma(void) {
    // remap DAC1 channel 1 requests
    // to DMA1 channel 3
    RCC_APB2ENR |= BIT0;
    SYSCFG_CFGR1 |= BIT13;

    //enable DMA1 clock
    RCC_AHBENR |= BIT0;

    DMA1_IFCR = 0;

    // Channel 3 initialization stuff

    // set size of peripherial register
    // to 8 bit
    DMA1_CCR3 &= ~(BIT8 | BIT9);
    // set size of memory
    // to be transfered to peripherial
    // register to 8 bit
    DMA1_CCR3 &= ~(BIT11 | BIT10);
    // disable TE/HT/TC interrupts
    DMA1_CCR3 &= ~(BIT1 | BIT2 | BIT3);
    // set high priority for channel
    DMA1_CCR3 |= BIT13;
    DMA1_CCR3 &= ~BIT12;
    // disable MEM2MEM mode
    DMA1_CCR3 &= ~BIT14;
    // enable incrementation of memory
    DMA1_CCR3 |= BIT7;
    // disable incrementation of peripherial
    DMA1_CCR3 &= ~BIT6;
    // enable circular mode
    DMA1_CCR3 |= BIT5;
    // set memory->peripherial read mode
    DMA1_CCR3 |= BIT4;
    // set number of data to be transmitted
    // to number of sine samples
    DMA1_CNDTR3 = SAMPLES_PER_WAVE;
    // set peripherial address to
    // DAC1 channel 1
    DMA1_CPAR3 = (uint32_t) &DAC_DHR8R1;
    // set memory address to
    // array of sine samples
    DMA1_CMAR3 = get_sine_wave();
    // enable channel
    DMA1_CCR3 |= BIT0;
}

/**
 * @file core.h
 * @author Yevheniy Verzun
 * @date August 2017
 * @brief ARM Cortex-m4 core peripherals defines
 *
 * Defines memory addresses of core peripherals and registers: systick (STK),
 * system control block (SCB), nested vectored interrupt controller (NVIC),
 * floationg point unit (FPU)
 */
#ifndef CORE_H
#define CORE_H

#include "common.h"

//! Macros to enable/disable global interrupts
#define enable_interrupts()                 asm volatile("cpsie i")
#define disable_interrupts()                asm volatile("cpsid i")

//! Core peripherals
#define STK_BASE                            0xe000e010
#define SCB_BASE                            0xe000ed00
#define NVIC_BASE                           0xe000e100
#define SCB_BASE2                           0xe000e008
#define NVIC_BASE2                          0xe000ef00 

//! STK
#define STK_CSR                             REGISTER_32(STK_BASE + 0)
#define STK_RVR                             REGISTER_32(STK_BASE + 4)
#define STK_CVR                             REGISTER_32(STK_BASE + 8)
#define STK_CALIB                           REGISTER_32(STK_BASE + 0x0c)

//! SCB_BASE
#define CPUID                               REGISTER_32(SCB_BASE + 0)
#define ICSR                                REGISTER_32(SCB_BASE + 4)
#define AIRCR                               REGISTER_32(SCB_BASE + 0x0c)
#define SCR                                 REGISTER_32(SCB_BASE + 0x10)
#define CCR                                 REGISTER_32(SCB_BASE + 0x14)
#define SHPR1                               REGISTER_32(SCB_BASE + 0x18)
#define SHPR2                               REGISTER_32(SCB_BASE + 0x1c)
#define SHPR3                               REGISTER_32(SCB_BASE + 0x20)
#define CFSR                                REGISTER_32(SCB_BASE + 0x28)
#define HFSR                                REGISTER_32(SCB_BASE + 0x2C)
#define BFAR                                REGISTER_32(SCB_BASE + 0x38)

//! NVIC
#define ISER0                               REGISTER_32(NVIC_BASE + 0x000)
#define ISER1                               REGISTER_32(NVIC_BASE + 0x004)
#define ISER2                               REGISTER_32(NVIC_BASE + 0x008)
#define ICER0                               REGISTER_32(NVIC_BASE + 0x080)
#define ICER1                               REGISTER_32(NVIC_BASE + 0x084)
#define ICER2                               REGISTER_32(NVIC_BASE + 0x088)
#define ISPR0                               REGISTER_32(NVIC_BASE + 0x100)
#define ISPR1                               REGISTER_32(NVIC_BASE + 0x104)
#define ISPR2                               REGISTER_32(NVIC_BASE + 0x108)
#define ICPR0                               REGISTER_32(NVIC_BASE + 0x180)
#define ICPR1                               REGISTER_32(NVIC_BASE + 0x184)
#define ICPR2                               REGISTER_32(NVIC_BASE + 0x188)
#define IABR0                               REGISTER_32(NVIC_BASE + 0x200)
#define IABR1                               REGISTER_32(NVIC_BASE + 0x204)
#define IABR2                               REGISTER_32(NVIC_BASE + 0x208)
#define IPR0                                REGISTER_32(NVIC_BASE + 0x300)
#define IPR1                                REGISTER_32(NVIC_BASE + 0x304)
#define IPR2                                REGISTER_32(NVIC_BASE + 0x308)
#define IPR3                                REGISTER_32(NVIC_BASE + 0x30c)
#define IPR4                                REGISTER_32(NVIC_BASE + 0x310)
#define IPR5                                REGISTER_32(NVIC_BASE + 0x314)
#define IPR6                                REGISTER_32(NVIC_BASE + 0x318)
#define IPR7                                REGISTER_32(NVIC_BASE + 0x31c)
#define IPR8                                REGISTER_32(NVIC_BASE + 0x320)
#define IPR9                                REGISTER_32(NVIC_BASE + 0x324)
#define IPR10                               REGISTER_32(NVIC_BASE + 0x328)
#define IPR11                               REGISTER_32(NVIC_BASE + 0x32c)
#define IPR12                               REGISTER_32(NVIC_BASE + 0x330)
#define IPR13                               REGISTER_32(NVIC_BASE + 0x334)
#define IPR14                               REGISTER_32(NVIC_BASE + 0x338)
#define IPR15                               REGISTER_32(NVIC_BASE + 0x33c)
#define IPR16                               REGISTER_32(NVIC_BASE + 0x340)
#define IPR17                               REGISTER_32(NVIC_BASE + 0x344)
#define IPR18                               REGISTER_32(NVIC_BASE + 0x348)
#define IPR19                               REGISTER_32(NVIC_BASE + 0x34c)
#define IPR20                               REGISTER_32(NVIC_BASE + 0x350)
#define IPR21                               REGISTER_32(NVIC_BASE + 0x354)
#define IPR22                               REGISTER_32(NVIC_BASE + 0x358)
#define IPR23                               REGISTER_32(NVIC_BASE + 0x35c)
#define IPR24                               REGISTER_32(NVIC_BASE + 0x360)
#define IPR25                               REGISTER_32(NVIC_BASE + 0x364)

#endif /* CORE_H */

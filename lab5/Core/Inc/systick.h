/*
 * systick.h
 *
 *  Created on: Jan 27, 2025
 *      Author: Phat
 */
#pragma once
#ifndef LIB_SYSTICK_H_
#define LIB_SYSTICK_H_
#include <stdint.h>

#define SYSTICK_CSR_BASE            0xE000E010
#define SYSTICK_BASE                SYSTICK_CSR_BASE

#define INTERNAL_CLOCK          (1U << 2)
#define COUNTER_EN              (1U << 0)
#define COUNTFLAG               (1U<<16)
#define INTERRUPT_SYSTICK_EN    (1U << 1)


typedef struct
{
    volatile uint32_t SYST_CSR; // control and status register
    volatile uint32_t SYST_RVR; // reload value register
    volatile uint32_t SYST_CVR; // current value register
    volatile uint32_t SYST_CALIB; // calibration value register
}Systick_config_t;

#define SYSTICK                    ((Systick_config_t*)SYSTICK_BASE)
void delay_ms(int ms);
void create_1hz_interrupt_systick();


#endif /* LIB_SYSTICK_H_ */

#ifndef CLOCK_H
#define CLOCK_H

#include "stm32f1xx.h"

void enable_HSI_as_sysclock_src();
void enable_HSE_as_sysclock_src();
void enable_PLL_sysclock_src_external_HSE_PLL_src();

void config_systic (uint32_t ticks);
void config_systic_for_ms (uint32_t sysclock_freq_input);

void Delay(uint32_t nTime);

#endif

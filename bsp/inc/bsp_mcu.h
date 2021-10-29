#ifndef __BSP_MCU_H
#define __BSP_MCU_H

#include "CH57x_common.h"

void mcu_power_init();
void mcu_timer_init();
void mcu_sleep_init();
void mcu_debug_uart_init();
uint32_t mcu_enter_lowpower(uint32_t time);
uint16_t mcu_get_internal_temp();

#endif

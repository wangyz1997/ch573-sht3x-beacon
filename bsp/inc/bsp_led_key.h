#ifndef __BSP_LED_KEY_H
#define __BSP_LED_KEY_H

#include "CH57x_common.h"
#include "marco_util.h"

#define LED_GPIO GPIOA
#define LED_GPIO_PIN 13
#define LED_ACTIVE_LEVEL 1

#define LED_GPIO_INIT() GPIOx_ModeCfg(LED_GPIO, (1<<LED_GPIO_PIN), GPIO_ModeOut_PP_5mA)
#define LED_TOGGLE() GPIOx_InverseBits(LED_GPIO, (1<<LED_GPIO_PIN))

#if LED_ACTIVE_LEVEL
#define LED_ON() GPIOx_SetBits(LED_GPIO, (1<<LED_GPIO_PIN))
#define LED_OFF() GPIOx_ResetBits(LED_GPIO, (1<<LED_GPIO_PIN))
#else
#define LED_ON() GPIOx_ResetBits(LED_GPIO, (1<<LED_GPIO_PIN))
#define LED_OFF() GPIOx_SetBits(LED_GPIO, (1<<LED_GPIO_PIN))
#endif

void led_init();

#endif


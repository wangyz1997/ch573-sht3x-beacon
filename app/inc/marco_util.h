#ifndef __MARCO_UTIL_H
#define __MARCO_UTIL_H

#define CONCAT2(a,b) a ## b

#define GPIOx_SetBits(gpiox, pin) CONCAT2(gpiox ,_SetBits)(pin)
#define GPIOx_ResetBits(gpiox, pin) CONCAT2(gpiox, _ResetBits)(pin)
#define GPIOx_InverseBits(gpiox, pin) CONCAT2(gpiox, _InverseBits)(pin)
#define GPIOx_ReadPortPin(gpiox, pin) CONCAT2(gpiox, _ReadPortPin)(pin)
#define GPIOx_ModeCfg(gpiox, pin, mode) CONCAT2(gpiox, _ModeCfg)(pin, mode)

#endif

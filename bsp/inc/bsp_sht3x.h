#ifndef __BSP_SHT3X_H
#define __BSP_SHT3X_H

#include "CH57x_common.h"

#define SHT3X_I2C_ADDR 0x44

typedef enum {
    sht3x_conversion_period_long = 0x2400, //长转换周期 12.5ms
    sht3x_conversion_period_medium = 0x240B, //中等转换周期 4.5ms
    sht3x_conversion_period_short = 0x2416 //短转换周期 2.5ms
} sht3x_conversion_period_t;

uint8_t sht3x_init();
uint8_t sht3x_trigger(sht3x_conversion_period_t period);
uint8_t sht3x_fetch(int16_t *temp, int16_t *humid);


#endif

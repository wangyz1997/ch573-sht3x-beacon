#ifndef __BSP_SW_I2C_H
#define __BSP_SW_I2C_H

#include "CH57x_common.h"
#include "marco_util.h"

#define I2C_GPIO GPIOB
#define I2C_SCL_PIN GPIO_Pin_12
#define I2C_SDA_PIN GPIO_Pin_13

#define I2C_SCL_1()        GPIOx_SetBits(I2C_GPIO, I2C_SCL_PIN)
#define I2C_SCL_0()        GPIOx_ResetBits(I2C_GPIO, I2C_SCL_PIN)
#define I2C_SCL_MODE_IN()  GPIOx_ModeCfg(I2C_GPIO, I2C_SCL_PIN, GPIO_ModeIN_Floating)
#define I2C_SCL_MODE_OUT() GPIOx_ModeCfg(I2C_GPIO, I2C_SCL_PIN, GPIO_ModeOut_PP_5mA)
#define I2C_SCL_READ()     GPIOx_ReadPortPin(I2C_GPIO, I2C_SCL_PIN)

#define I2C_SDA_1()        GPIOx_SetBits(I2C_GPIO, I2C_SDA_PIN)
#define I2C_SDA_0()        GPIOx_ResetBits(I2C_GPIO, I2C_SDA_PIN)
#define I2C_SDA_MODE_IN()  GPIOx_ModeCfg(I2C_GPIO, I2C_SDA_PIN, GPIO_ModeIN_Floating)
#define I2C_SDA_MODE_OUT() GPIOx_ModeCfg(I2C_GPIO, I2C_SDA_PIN, GPIO_ModeOut_PP_5mA)
#define I2C_SDA_READ()     GPIOx_ReadPortPin(I2C_GPIO, I2C_SDA_PIN)

void i2c_init();
void i2c_start(void);
void i2c_stop(void);
uint8_t i2c_write_byte(uint8_t dat);
uint8_t i2c_read_byte(uint8_t ack);
uint8_t i2c_check(uint8_t addr);

#endif

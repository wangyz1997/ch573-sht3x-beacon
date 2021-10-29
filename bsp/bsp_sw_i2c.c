#include "bsp_sw_i2c.h"

/**
 * @brief I2C通信延时函数
 *
 */
void i2c_delay()
{
    __nop(); __nop(); __nop(); //约400kHz
}

/**
 * @brief 初始化I2C总线
 *
 */
void i2c_init()
{
    I2C_SDA_1();
    I2C_SCL_1();
    I2C_SCL_MODE_OUT();
    I2C_SDA_MODE_OUT();
}

/**
 * @brief 产生I2C开始信号
 *
 */
void i2c_start()
{
    I2C_SDA_MODE_OUT();
    I2C_SDA_1();
    I2C_SCL_1();
    i2c_delay();
    I2C_SDA_0();
    i2c_delay();
    I2C_SCL_0();
    i2c_delay();
}

/**
 * @brief 产生I2C停止信号
 *
 */
void i2c_stop()
{
    I2C_SDA_MODE_OUT();
    I2C_SCL_0();
	I2C_SDA_0();
    i2c_delay();
	I2C_SCL_1();
	i2c_delay();
	I2C_SDA_1();
}

/**
 * @brief I2C写时等待从机ACK信号
 *
 * @return uint8_t 0 从机正确发送ACK
 *                 1 等待从机发送ACK超时
 */
static uint8_t i2c_wait_ack()
{
    uint16_t timeout = 0;

    I2C_SDA_MODE_IN(); //释放SDA

    I2C_SCL_0();
    i2c_delay();
    I2C_SCL_1(); //第9个位时钟

    while(I2C_SDA_READ()) {
        timeout ++;
        if(timeout > 10000) {
            i2c_stop(); //超时
            return 1;
        }
    }

    I2C_SCL_0();

    return 0;
} 

/**
 * @brief I2C读时产生ACK或NAK信号
 *
 * @param ack 0为产生NAK信号，1为产生ACK信号
 */
static void i2c_gen_ack(uint8_t ack)
{
    I2C_SCL_0();

    if(ack) {
        I2C_SDA_0(); //ACK
    } else {
        I2C_SDA_1(); //NAK
    }
    I2C_SDA_MODE_OUT();
    i2c_delay();
    I2C_SCL_1(); //第9个位时钟
    i2c_delay();

    I2C_SCL_0();
}

/**
 * @brief 使用I2C发送一个字节
 *
 * @param dat 要发送的数据
 * @return uint8_t 0 从机正确发送ACK
 *                 1 等待从机发送ACK超时
 */
uint8_t i2c_write_byte(uint8_t dat)
{
    I2C_SDA_MODE_OUT();

    for(uint8_t i = 0; i < 8; i ++) {
        I2C_SCL_0();

        if(dat & 0x80) {
            I2C_SDA_1();
        }
        else {
            I2C_SDA_0();
        }
        dat <<= 1;
        i2c_delay();

        I2C_SCL_1();
        i2c_delay(); //上升沿数据有效
    }

    I2C_SCL_0();

    return i2c_wait_ack();
}

/**
 * @brief 使用I2C接收一个字节
 *
 * @param ack 0为不产生NAK信号，1为产生ACK信号
 * @return uint8_t 读取到的数据
 */
uint8_t i2c_read_byte(uint8_t ack)
{
    uint8_t receive;

    I2C_SDA_MODE_IN();

    for(uint8_t i = 0; i < 8; i ++) {
        I2C_SCL_0();
        i2c_delay();

        receive <<= 1;

        I2C_SCL_1();
        i2c_delay();

        if(I2C_SDA_READ()) {
            receive |= 0x01;
        } else {
            receive &= ~0x01; //保证时钟的高低时长相等
        }
    }

    I2C_SCL_0();

    i2c_gen_ack(ack);

    return receive;
}

/**
 * @brief 检测I2C上某个地址是否存在
 *
 * @param addr 要检测的地址
 * @return uint8_t 1 该地址存在设备
 *                 0 该地址不存在设备
 */
uint8_t i2c_check(uint8_t addr)
{
    uint8_t res;

    i2c_start();
    res = i2c_write_byte(addr<<1 | 0);
    i2c_stop();

    return !res;
}

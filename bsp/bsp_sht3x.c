#include "bsp_sht3x.h"

#include "bsp_sw_i2c.h"

/* CRC的生成多项式 */
#define POLYNOMIAL 0x131 // P(x) = x^8 + x^5 + x^4 + 1 = 100110001

/**
 * @brief 使用给定多项式计算CRC8
 *
 * @param data 输入数据
 * @param len 输入数据长度
 * @return uint8_t CRC8校验值
 */
static uint8_t sht3x_cal_crc(uint8_t *data, uint8_t len)
{
    uint8_t crc = 0xFF;

    for (uint8_t i = 0; i < len; i++) {
        crc ^= (data[i]);
        for (uint8_t bit = 8; bit > 0; --bit) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ POLYNOMIAL;
            } else {
                crc = (crc << 1);
            }
        }
    }

    return crc;
}

/**
 * @brief 对接收到的数据进行CRC8校验
 *
 * @param data 接收到的数据缓冲区
 * @param len 接收到的数据长度
 * @return uint8_t 0 读取成功
 *                 1 CRC校验失败
 *                 2 输入数据长度不是3的倍数或输入为空
 */
static uint8_t sht3x_check_crc(uint8_t *data, uint8_t len)
{
    /* 检测输入数据长度是否为3的整数倍 即是否为16位数据+8位CRC校验 */
    if (len % 3 || len == 0) {
        return 2;
    }

    for (int i = 0; i < len; i += 3) {
        uint8_t crc = sht3x_cal_crc(data + i, 2);
        if (crc != data[i + 2]) {
            return 1;
        }
    }

    return 0;
}

/**
 * @brief 通过I2C总线向SHT3x的寄存器写入数据
 *
 * @param reg 寄存器地址
 * @return uint8_t 0 写入成功
 *                 1 通讯失败
 */
static uint8_t sht3x_i2c_write_reg(uint16_t reg)
{
    i2c_start();
    if (i2c_write_byte((SHT3X_I2C_ADDR << 1) | 0)) {
        goto exit;
    }

    if (i2c_write_byte((reg >> 8) | 0)) {
        goto exit;
    }
    if (i2c_write_byte((reg & 0xFF) | 0)) {
        goto exit;
    }

    i2c_stop();
    return 0;

exit:
    i2c_stop();
    return 1;
}

/**
 * @brief 通过I2C总线从SHT3x读取数据
 *
 * @param buf 存储读取到数据的缓冲区
 * @param len 要读取数据的长度
 * @return uint8_t 0 读取成功
 *                 1 CRC校验失败
 *                 2 通信失败
 */
static uint8_t sht3x_i2c_read(uint8_t *buf, uint8_t len)
{
    i2c_start();
    if (i2c_write_byte((SHT3X_I2C_ADDR << 1) | 1)) {
        i2c_stop();
        return 2;
    }

    for (uint8_t i = 0; i < len - 1; i++) {
        buf[i] = i2c_read_byte(1); // ACK
    }
    buf[len - 1] = i2c_read_byte(0); // NAK

    i2c_stop();

    if (sht3x_check_crc(buf, len)) {
        return 1;
    }

    return 0;
}

/**
 * @brief 计算温度
 *
 * @param raw 温度寄存器原始值
 * @return int16_t 摄氏温度的100倍
 */
static int16_t sht3x_calc_temp(uint16_t raw) { return 17500l * (int32_t)raw / 65535 - 4500; }

/**
 * @brief 计算相对湿度
 *
 * @param raw 湿度寄存器原始值
 * @return uint16_t 相对湿度的100倍
 */
static int16_t sht3x_calc_humid(uint16_t raw) { return 10000l * (int32_t)raw / 65535; }

/**
 * @brief 软复位SHT3x
 *
 * @return uint8_t 0 复位成功
 *                 1 复位失败
 */
static uint8_t sht3x_soft_reset()
{
    /* 写入软复位寄存器 */
    uint8_t error = sht3x_i2c_write_reg(0x30A2);

    if (error != 0) {
        PRINT("error while resetting sht3x\r\n"); //复位失败
    } else {
        // DelayMs(50); //复位成功，延时50ms
    }

    return error;
}

/**
 * @brief 初始化SHT3x
 *
 * @return uint8_t 0 初始化成功
 *                 1 初始化失败
 */
uint8_t sht3x_init()
{
    i2c_init();

    /* 软复位SHT3x 并检测SHT3x是否存在 */
    uint8_t error = sht3x_soft_reset();
    if (error != 0) {
        PRINT("error while initializing sht3x\r\n");
        return error;
    }

    PRINT("sht3x initialized\r\n");
    return error;
}

/**
 * @brief 写入转换周期并启动转换。转换周期越长，能耗越高，但噪声越低。
 * 
 * @param period 转换周期选择
 * @return uint8_t 0 读取成功
 *                 1 读取失败
 */
uint8_t sht3x_trigger(sht3x_conversion_period_t period)
{
    uint8_t error = sht3x_i2c_write_reg(period); 
    if (error != 0) {
        PRINT("sht3x error while starting measurement\r\n");
    }
}

/**
 * @brief 从SHT3x读取温湿度
 *
 * @param temp [out]温度
 * @param humid [out]温度
 * @return uint8_t 0 读取成功
 *                 1 读取失败
 */
uint8_t sht3x_fetch(int16_t *temp, int16_t *humid)
{
    uint8_t result_buf[6];

    /* 读取并校验 */
    uint8_t error = sht3x_i2c_read(result_buf, sizeof(result_buf));

    if (error == 0) {
        /* 计算温度与湿度 */
        *temp = sht3x_calc_temp(result_buf[0] << 8 | result_buf[1]);
        *humid = sht3x_calc_humid(result_buf[3] << 8 | result_buf[4]);
    } else if (error == 1) {
        PRINT("sht3x crc check error\r\n");
    } else if (error == 2) {
        PRINT("sht3x communication error\r\n");
    }

    return error;
}

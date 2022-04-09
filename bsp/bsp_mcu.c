#include "bsp_mcu.h"

#include "bsp_config.h"
#include "CH57xBLE_LIB.h"

static volatile uint8_t rtc_trig_flag;

__attribute__((interrupt("WCH-Interrupt-fast")))
__attribute__((section(".highcode")))
void RTC_IRQHandler(void)
{
    R8_RTC_FLAG_CTRL = (RB_RTC_TMR_CLR | RB_RTC_TRIG_CLR);
    rtc_trig_flag = 1;
}

static void mcu_set_rtc_trigger_time(uint32_t time)
{
    R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG1;
    R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG2;
    R32_RTC_TRIG = time;
    rtc_trig_flag = 0;
}

/*******************************************************************************
 * @fn          CH57X_LowPower
 *
 * @brief       启动睡眠
 *
 * input parameters
 *
 * @param       time-唤醒的时间点（RTC绝对值）
 *
 * output parameters
 *
 * @param
 *
 * @return      None.
 */
uint32_t mcu_enter_lowpower(uint32_t time)
{
#if (defined(HAL_SLEEP)) && (HAL_SLEEP == TRUE)
    uint32_t tmp, irq_status;

    SYS_DisableAllIrq(&irq_status);
    tmp = RTC_GetCycle32k();
    if((time < tmp) || ((time - tmp) < 30)) { //检测睡眠的最短时间
        SYS_RecoverIrq(irq_status);
        return 2;
    }
    mcu_set_rtc_trigger_time(time);
    SYS_RecoverIrq(irq_status);
#if DEBUG == Debug_UART1 //等待串口FIFO发送完成
    while ((R8_UART1_LSR & RB_LSR_TX_ALL_EMP) == 0) {
        __nop();
    }
#endif
    //LOW POWER-sleep模式
    if(!rtc_trig_flag) {
        LowPower_Sleep(RB_PWR_RAM2K | RB_PWR_RAM16K | RB_PWR_EXTEND);
#if (defined(DCDC_ENABLE)) && (DCDC_ENABLE == TRUE)
        PWR_DCDCCfg(ENABLE);
#endif
        time += WAKE_UP_RTC_MAX_TIME;
        if (time > 0xA8C00000) time -= 0xA8C00000;
        mcu_set_rtc_trigger_time(time);
        LowPower_Idle();
        HSECFG_Current(HSE_RCur_100); //降为额定电流(低功耗函数中提升了HSE偏置电流)
    } else {
        return 3;
    }
#endif
    return 0;
}

void mcu_power_init()
{
    PWR_DCDCCfg(ENABLE);
    SetSysClock(CLK_SOURCE_PLL_60MHz);
    GPIOA_ResetBits(GPIO_Pin_All);
    GPIOB_ResetBits(GPIO_Pin_All);
    GPIOA_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_PD);
    GPIOB_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_PD);
}

void mcu_timer_init()
{
    R8_SAFE_ACCESS_SIG = 0x57;
    R8_SAFE_ACCESS_SIG = 0xa8;
    R8_CK32K_CONFIG |= RB_CLK_OSC32K_XT | RB_CLK_INT32K_PON | RB_CLK_XT32K_PON;
    R8_SAFE_ACCESS_SIG = 0;

    RTC_InitTime(2020, 1, 1, 0, 0, 0); //RTC时钟初始化当前时间
    TMOS_TimerInit(0);
}

void mcu_debug_uart_init()
{
    GPIOA_SetBits(bTXD1);
    GPIOA_ModeCfg(bTXD1, GPIO_ModeOut_PP_5mA);
    UART1_DefInit();
}

void mcu_sleep_init()
{
    R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG1;
    R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG2;
    R8_SLP_WAKE_CTRL |= RB_SLP_RTC_WAKE; //RTC唤醒
    R8_RTC_MODE_CTRL |= RB_RTC_TRIG_EN;  //触发模式
    R8_SAFE_ACCESS_SIG = 0;
    PFIC_EnableIRQ(RTC_IRQn);
}

uint16_t mcu_get_internal_temp()
{
    uint8_t sensor, channel, config, tkey_cfg;
    uint16_t adc_data;

    tkey_cfg = R8_TKEY_CFG;
    sensor = R8_TEM_SENSOR;
    channel = R8_ADC_CHANNEL;
    config = R8_ADC_CFG;
    ADC_InterTSSampInit();
    R8_ADC_CONVERT |= RB_ADC_START;
    while(R8_ADC_CONVERT & RB_ADC_START);
    adc_data = R16_ADC_DATA;
    R8_TEM_SENSOR = sensor;
    R8_ADC_CHANNEL = channel;
    R8_ADC_CFG = config;
    R8_TKEY_CFG = tkey_cfg;
    return (adc_data);
}

/********************************** (C) COPYRIGHT *******************************
* File Name          : CH57x_pwr.c
* Author             : WCH
* Version            : V1.0
* Date               : 2018/12/15
* Description 
*******************************************************************************/

#include "CH57x_common.h"


/*******************************************************************************
* Function Name  : PWR_DCDCCfg
* Description    : 启用内部DC/DC电源，用于节约系统功耗
* Input          : s:  
                    ENABLE  - 打开DCDC电源
                    DISABLE - 关闭DCDC电源   				
* Return         : None
*******************************************************************************/
void PWR_DCDCCfg( FunctionalState s )
{
    if(s == DISABLE)
    {		
        R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG1;		
        R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG2;
        R16_POWER_PLAN &= ~(RB_PWR_DCDC_EN|RB_PWR_DCDC_PRE);		// 旁路 DC/DC 
        R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG0;		
    }
    else
    {
        UINT32 HW_Data[2];
        FLASH_EEPROM_CMD( CMD_GET_ROM_INFO, ROM_CFG_ADR_HW, HW_Data, 0 );
        if( (HW_Data[0]) & (1<<13) )  return;
        R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG1;		
        R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG2;
        R16_POWER_PLAN |= RB_PWR_DCDC_PRE;
        R16_POWER_PLAN |= RB_PWR_DCDC_EN;		
        R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG0;			
    }
}

/*******************************************************************************
* Function Name  : PWR_UnitModCfg
* Description    : 可控单元模块的电源控制
* Input          : s:  
                    ENABLE  - 打开   
                    DISABLE - 关闭
                   unit:
                    please refer to unit of controllable power supply 				
* Return         : None
*******************************************************************************/
void PWR_UnitModCfg( FunctionalState s, UINT8 unit )
{
    if(s == DISABLE)		//关闭
    {
    	R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG1;		
    	R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG2;
        R8_HFCK_PWR_CTRL &= ~(unit&0x1c);
        R8_CK32K_CONFIG &= ~(unit&0x03);
    }
    else					//打开
    {
    	R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG1;		
    	R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG2;
        R8_HFCK_PWR_CTRL |= (unit&0x1c);
        R8_CK32K_CONFIG |= (unit&0x03);
    }
    R8_SAFE_ACCESS_SIG = 0;
}

/*******************************************************************************
* Function Name  : PWR_PeriphClkCfg
* Description    : 外设时钟控制位
* Input          : s:  
                    ENABLE  - 打开外设时钟   
                    DISABLE - 关闭外设时钟
                   perph:
                    please refer to Peripher CLK control bit define						
* Return         : None
*******************************************************************************/
void PWR_PeriphClkCfg( FunctionalState s, UINT16 perph )
{
    if( s == DISABLE )
    {
        R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG1;		
        R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG2;
        R32_SLEEP_CONTROL |= perph;
    }
    else
    {
        R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG1;		
        R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG2;
        R32_SLEEP_CONTROL &= ~perph;
    }
    R8_SAFE_ACCESS_SIG = 0;
}

/*******************************************************************************
* Function Name  : PWR_PeriphWakeUpCfg
* Description    : 睡眠唤醒源配置
* Input          : s:  
                    ENABLE  - 打开此外设睡眠唤醒功能   
                    DISABLE - 关闭此外设睡眠唤醒功能
                   perph:
                    RB_SLP_USB_WAKE	    -  USB 为唤醒源
                    RB_SLP_RTC_WAKE	    -  RTC 为唤醒源
                    RB_SLP_GPIO_WAKE	  -  GPIO 为唤醒源
                    RB_SLP_BAT_WAKE	    -  BAT 为唤醒源
                   mode: refer to WakeUP_ModeypeDef
* Return         : None
*******************************************************************************/
void PWR_PeriphWakeUpCfg( FunctionalState s, UINT8 perph, WakeUP_ModeypeDef mode )
{
	UINT8  m;

    if( s == DISABLE )
    {
        R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG1;		
        R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG2;
        R8_SLP_WAKE_CTRL &= ~perph;		
    }
    else
    {
    	switch( mode )
    	{
    	case Edge_LongDelay:
    		m = RB_WAKE_EV_MODE;
    		break;

    	case Level_LongDelay:
    		m = 0;
    		break;

    	case Level_ShortDelay:
    		m = RB_WAKE_DELAY;
    		break;

    	default:
    		m = RB_WAKE_EV_MODE|RB_WAKE_DELAY;
    		break;
    	}
        R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG1;		
        R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG2;
        R8_SLP_WAKE_CTRL &= ~(RB_WAKE_EV_MODE|RB_WAKE_DELAY);
        R8_SLP_WAKE_CTRL |= m | perph;
    }
    R8_SAFE_ACCESS_SIG = 0;
}

/*******************************************************************************
* Function Name  : PowerMonitor
* Description    : 电源监控
* Input          : s:  
                    ENABLE  - 打开此功能   
                    DISABLE - 关闭此功能
                   vl: refer to VolM_LevelypeDef
* Return         : None
*******************************************************************************/
void PowerMonitor( FunctionalState s , VolM_LevelypeDef vl)
{
    if( s == DISABLE )
    {
        R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG1;		
        R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG2;
        R8_BAT_DET_CTRL = 0;
        R8_SAFE_ACCESS_SIG = 0; 
    }
    else
    {
    	R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG1;
    	R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG2;
    	if( vl & 0x80 )
    	{
    		R8_BAT_DET_CFG = vl&0x03;
    		R8_BAT_DET_CTRL = RB_BAT_MON_EN|((vl>>2)&1);
    	}
    	else
    	{
    		R8_BAT_DET_CFG = vl&0x03;
    		R8_BAT_DET_CTRL = RB_BAT_DET_EN;
    	}
        R8_SAFE_ACCESS_SIG = 0; 
        mDelayuS(1); 	
        R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG1;		
        R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG2;
        R8_BAT_DET_CTRL |= RB_BAT_LOW_IE|RB_BAT_LOWER_IE;
        R8_SAFE_ACCESS_SIG = 0;  
    }   
}

/*******************************************************************************
* Function Name  : LowPower_Idle
* Description    : 低功耗-Idle模式
* Input          : None
* Return         : None
*******************************************************************************/
__attribute__((section(".highcode")))
void LowPower_Idle( void )
{
  FLASH_ROM_SW_RESET();
  R8_FLASH_CTRL = 0x04;   //flash关闭

	PFIC -> SCTLR &= ~(1<<2);				// sleep
	__WFI();
	__nop();__nop();
}

/*******************************************************************************
* Function Name  : LowPower_Halt
* Description    : 低功耗-Halt模式。
                   此低功耗切到HSI/5时钟运行，唤醒后需要用户自己重新选择系统时钟源
* Input          : None
* Return         : None
*******************************************************************************/
__attribute__((section(".highcode")))
void LowPower_Halt( void )
{
    UINT8  x32Kpw, x32Mpw;
    
    FLASH_ROM_SW_RESET();
    R8_FLASH_CTRL = 0x04;   //flash关闭
    x32Kpw = R8_XT32K_TUNE;
    x32Mpw = R8_XT32M_TUNE;
    x32Mpw = (x32Mpw&0xfc)|0x03;            // 150%额定电流
    if(R16_RTC_CNT_32K>0x3fff){     // 超过500ms
        x32Kpw = (x32Kpw&0xfc)|0x01;        // LSE驱动电流降低到额定电流
    }
    
    R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG1;		
    R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG2;
    R8_BAT_DET_CTRL = 0;                              // 关闭电压监控
    R8_XT32K_TUNE = x32Kpw;
    R8_XT32M_TUNE = x32Mpw;
    R8_PLL_CONFIG |= (1<<5);
    R8_SAFE_ACCESS_SIG = 0;
        
    PFIC -> SCTLR |= (1<<2);				//deep sleep
    __WFI();
    __nop();__nop();
    R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG1;
    R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG2;
    R8_PLL_CONFIG &= ~(1<<5);
    R8_SAFE_ACCESS_SIG = 0;
}

/*******************************************************************************
* Function Name  : LowPower_Sleep
* Description    : 低功耗-Sleep模式。
                   此低功耗切到HSI/5时钟运行，唤醒后需要用户自己重新选择系统时钟源
                   注意调用此函数，DCDC功能强制关闭，唤醒后可以手动再次打开
* Input          : rm:
                    RB_PWR_RAM2K	-	2K retention SRAM 供电
                    RB_PWR_RAM16K	-	16K main SRAM 供电
                    RB_PWR_EXTEND	-	USB 和 BLE 单元保留区域供电
                    RB_PWR_XROM   - FlashROM 供电
                   NULL	-	以上单元都断电
* Return         : None
*******************************************************************************/
__attribute__((section(".highcode")))
void LowPower_Sleep( UINT8 rm )
{
    UINT8  x32Kpw, x32Mpw;
    
    x32Kpw = R8_XT32K_TUNE;
    x32Mpw = R8_XT32M_TUNE;
    x32Mpw = (x32Mpw&0xfc)|0x03;            // 150%额定电流
    if(R16_RTC_CNT_32K>0x3fff){     // 超过500ms
        x32Kpw = (x32Kpw&0xfc)|0x01;        // LSE驱动电流降低到额定电流
    } 

    R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG1;		
    R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG2;
    R8_BAT_DET_CTRL = 0;                // 关闭电压监控
    R8_XT32K_TUNE = x32Kpw;
    R8_XT32M_TUNE = x32Mpw;
    R8_SAFE_ACCESS_SIG = 0;

    PFIC -> SCTLR |= (1<<2);				//deep sleep

    R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG1;
    R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG2;
    R8_SLP_POWER_CTRL |= RB_RAM_RET_LV;
    R8_PLL_CONFIG |= (1<<5);
    R16_POWER_PLAN = RB_PWR_PLAN_EN       \
                    |RB_PWR_MUST_0010   \
                    |RB_PWR_CORE            \
                    |rm;
    __WFI();
    __nop();__nop();
    R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG1;
    R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG2;
    R8_PLL_CONFIG &= ~(1<<5);
    R8_SAFE_ACCESS_SIG = 0;
}

/*******************************************************************************
* Function Name  : LowPower_Shutdown
* Description    : 低功耗-Shutdown模式。
                   此低功耗切到HSI/5时钟运行，唤醒后需要用户自己重新选择系统时钟源
                   注意调用此函数，DCDC功能强制关闭，唤醒后可以手动再次打开
* Input          : rm:
                    RB_PWR_RAM2K  - 2K retention SRAM 供电
                    RB_PWR_RAM16K - 16K main SRAM 供电
                   NULL	-	以上单元都断电
* Return         : None
*******************************************************************************/
__attribute__((section(".highcode")))
void LowPower_Shutdown( UINT8 rm )
{	
    UINT8  x32Kpw, x32Mpw;
    
    x32Kpw = R8_XT32K_TUNE;
    x32Mpw = R8_XT32M_TUNE;
    x32Mpw = (x32Mpw&0xfc)|0x03;            // 150%额定电流
    if(R16_RTC_CNT_32K>0x3fff){     // 超过500ms
        x32Kpw = (x32Kpw&0xfc)|0x01;        // LSE驱动电流降低到额定电流
    }

    R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG1;		
    R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG2;
    R8_BAT_DET_CTRL = 0;                // 关闭电压监控
    R8_XT32K_TUNE = x32Kpw;
    R8_XT32M_TUNE = x32Mpw;
    R8_SAFE_ACCESS_SIG = 0;    
    SetSysClock( CLK_SOURCE_HSE_6_4MHz );

    PFIC -> SCTLR |= (1<<2);				//deep sleep

    R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG1;
    R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG2;
    R8_SLP_POWER_CTRL |= RB_RAM_RET_LV;
    R16_POWER_PLAN = RB_PWR_PLAN_EN       \
                    |RB_PWR_MUST_0010   \
                    |rm;
    __WFI();
    __nop();__nop();
}





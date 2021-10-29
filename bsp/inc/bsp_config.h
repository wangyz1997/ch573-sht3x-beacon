#ifndef __BSP_CONFIG_H
#define __BSP_CONFIG_H

#define	ID_CH573							0x73
#define CHIP_ID								ID_CH573

/*********************************************************************
 【MAC】
 BLE_MAC											- 是否自定义蓝牙Mac地址 ( 默认:FALSE - 使用芯片Mac地址 )，需要在main.c修改Mac地址定义

 【DCDC】
 DCDC_ENABLE                  - 是否使能DCDC ( 默认:FALSE )

 【SLEEP】
 HAL_SLEEP   									- 是否开启睡眠功能 ( 默认:FALSE )
 WAKE_UP_RTC_MAX_TIME					- 等待32M晶振稳定时间，根据不同睡眠类型取值可分为： 睡眠模式/下电模式         -	45(默认)
                                                                                                                                                                                        暂停模式					-	45
                                                                                                                                                                                        空闲模式					-	5
 
 【TEMPERATION】
 TEM_SAMPLE										- 是否打开根据温度变化校准的功能，单次校准耗时小于10ms( 默认:TRUE )
 
 【CALIBRATION】
 BLE_CALIBRATION_ENABLE				- 是否打开定时校准的功能，单次校准耗时小于10ms( 默认:TRUE )
 BLE_CALIBRATION_PERIOD				- 定时校准的周期，单位ms( 默认:120000 ) 
 
 【SNV】
 BLE_SNV    				          - 是否开启SNV功能，用于储存绑定信息( 默认:TRUE )
 BLE_SNV_ADDR    					    - SNV信息保存地址，使用data flash最后( 默认:0x77E00 )

 【RTC】
 CLK_OSC32K										- RTC时钟选择，如包含主机角色必须使用外部32K( 默认:0 外部(32768Hz)，1：内部(32000Hz)，2：内部(32768Hz) )

 【MEMORY】
 BLE_MEMHEAP_SIZE  						- 蓝牙协议栈使用的RAM大小，不小于6K ( 默认:(1024*6) )

 【DATA】
 BLE_BUFF_MAX_LEN							- 单个连接最大包长度( 默认:27 (ATT_MTU=23)，取值范围[27~251] )
 BLE_BUFF_NUM									- 控制器缓存的包数量( 默认:5 )
 BLE_TX_NUM_EVENT							- 单个连接事件最多可以发多少个数据包( 默认:1 )
 BLE_TX_POWER									- 发射功率( 默认:LL_TX_POWEER_0_DBM (0dBm) )
 
 【MULTICONN】
 PERIPHERAL_MAX_CONNECTION	  - 最多可同时做多少从机角色( 默认:1 )
 CENTRAL_MAX_CONNECTION				- 最多可同时做多少主机角色( 默认:3 )
 **********************************************************************/

/*********************************************************************
 * 默认配置值
 */

/* 是否开启DC-DC，需要硬件电路支持 */
#define DCDC_ENABLE TRUE

/* 是否启用低功耗睡眠功能，使能后无法调试 */
#define HAL_SLEEP TRUE

/* RTC时钟选择，如包含主机角色必须使用外部32K。0：外部(32768Hz)，1：内部(32000Hz)，2：内部(32768Hz) */
#define CLK_OSC32K  0

/* BLE协议栈内存大小 */
#define BLE_MEM_POOL_SIZE (1024*6)

/* 传感器检测周期，单位为秒 */
#define SENSOR_UPDATE_PERIOD 5

#define WAKE_UP_RTC_MAX_TIME				45

#define BLE_CALIBRATION_PERIOD			120000

#ifndef BLE_BUFF_MAX_LEN
#define BLE_BUFF_MAX_LEN						27
#endif
#ifndef BLE_BUFF_NUM
#define BLE_BUFF_NUM								5
#endif
#ifndef BLE_TX_NUM_EVENT
#define BLE_TX_NUM_EVENT						1
#endif

#define BLE_TX_POWER LL_TX_POWEER_0_DBM

#ifndef PERIPHERAL_MAX_CONNECTION
#define PERIPHERAL_MAX_CONNECTION		1
#endif
#ifndef CENTRAL_MAX_CONNECTION
#define CENTRAL_MAX_CONNECTION			3
#endif

#endif

/******************************* endfile @ config ******************************/

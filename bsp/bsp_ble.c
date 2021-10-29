#include "bsp_ble.h"

#include "bsp_config.h"
#include "bsp_mcu.h"
#include "CH57xBLE_LIB.h"

__attribute__((aligned(4))) uint32_t BLE_MEM_POOL[BLE_MEM_POOL_SIZE / 4];

void bsp_ble_init(void)
{
    bleConfig_t ble_cfg;
    if(tmos_memcmp(VER_LIB, VER_FILE, strlen(VER_FILE)) == FALSE) {
        PRINT("mismatched ble library version\r\n");
        while(1);
    }
    SysTick_Config(SysTick_LOAD_RELOAD_Msk);
    PFIC_DisableIRQ(SysTick_IRQn);

    tmos_memset(&ble_cfg, 0, sizeof(bleConfig_t));
    ble_cfg.MEMAddr = (uint32_t)BLE_MEM_POOL;
    ble_cfg.MEMLen = (uint32_t)BLE_MEM_POOL_SIZE;
    ble_cfg.BufMaxLen = (uint32_t)BLE_BUFF_MAX_LEN;
    ble_cfg.BufNumber = (uint32_t)BLE_BUFF_NUM;
    ble_cfg.TxNumEvent = (uint32_t)BLE_TX_NUM_EVENT;
    ble_cfg.TxPower = (uint32_t)BLE_TX_POWER;
    ble_cfg.SelRTCClock = (uint32_t)CLK_OSC32K;
    ble_cfg.ConnectNumber = (PERIPHERAL_MAX_CONNECTION & 3) | (CENTRAL_MAX_CONNECTION << 2);
    ble_cfg.srandCB = SYS_GetSysTickCnt;
    ble_cfg.tsCB = mcu_get_internal_temp; //根据温度变化校准RF和内部RC
#if (defined(HAL_SLEEP)) && (HAL_SLEEP == TRUE)
    ble_cfg.WakeUpTime = WAKE_UP_RTC_MAX_TIME;
    ble_cfg.sleepCB = mcu_enter_lowpower; // 启用睡眠
#endif

    uint8_t rom_mac_addr[6];
    GetMACAddress(rom_mac_addr);
    for(uint8_t i = 0; i < 6; i ++) {
        ble_cfg.MacAddr[i] = rom_mac_addr[i];
    }

    uint8_t error = BLE_LibInit(&ble_cfg);
    if(error) {
        PRINT("ble library initialization failed, code 0x%02x\r\n", error);
        while(1);
    }
    
    PRINT("ble initialization successful. library version: %s\r\n", VER_LIB);
}

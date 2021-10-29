#include "CH57x_common.h"
#include "bsp_config.h"
#include "bsp_ble.h"
#include "bsp_mcu.h"
#include "bsp_led_key.h"
#include "task_broadcaster.h"
#include "task_sensor.h"
#include "task_bsp.h"

__attribute__((section(".highcode")))
void tmos_circulation()
{
    while (1) {
        TMOS_SystemProcess();
    }
}

int main(void)
{
    mcu_power_init();
    led_init();
#ifdef DEBUG
    mcu_debug_uart_init();
#endif

    bsp_ble_init();
    GAPRole_BroadcasterInit();

    broadcaster_task_init();
    sensor_task_init();
    bsp_task_init();

    tmos_circulation();
}

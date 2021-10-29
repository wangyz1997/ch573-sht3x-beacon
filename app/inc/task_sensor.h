#ifndef __SENSOR_H
#define __SENSOR_H

/* TMOS消息事件定义 */
#define SENSOR_RESULT_EVENT 0x77 //传感器数值更新事件

#include "CH57xBLE_LIB.h"
#include "CH57x_common.h"

typedef struct {
    tmos_event_hdr_t hdr;
    int16_t temp;
    int16_t humid;
} sensor_event_data_t;

#endif

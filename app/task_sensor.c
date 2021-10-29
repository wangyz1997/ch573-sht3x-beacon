#include "task_sensor.h"

#include "bsp_config.h"
#include "bsp_sht3x.h"
#include "bsp_led_key.h"
#include "task_broadcaster.h"

/* TMOS事件定义 */
#define SENSOR_FINISH_EVENT 0x01 //传感器转换完成事件
#define SENSOR_TRIGGER_EVENT 0x02 //传感器数值定时触发事件

tmosTaskID sensor_task_id;

void sensor_trigger()
{
    /* 触发传感器转换 */
    sht3x_trigger(sht3x_conversion_period_medium);
    /* 设置转换完成事件的时间 */
    tmos_start_task(sensor_task_id, SENSOR_FINISH_EVENT, MS1_TO_SYSTEM_TIME(10));
}

void sensor_update()
{
    /* 申请消息内存 */
    sensor_event_data_t *pmsg = (sensor_event_data_t *)tmos_msg_allocate(sizeof(sensor_event_data_t));
    /* 消息头中指明消息类型为传感器更新消息 */
    pmsg->hdr.event = SENSOR_RESULT_EVENT; //传感器数值更新事件
    pmsg->hdr.status = 0;
    /* 取回传感器数据 */
    sht3x_fetch(&(pmsg->temp), &(pmsg->humid));
    tmos_msg_send(broadcaster_task_id, (uint8_t *)pmsg);
}

uint16_t sensor_event_handler(uint8_t task_id, uint16_t events)
{
    if(events & SYS_EVENT_MSG) //是系统消息
    {
        uint8_t *pMsg = tmos_msg_receive(sensor_task_id);

        if (pMsg != NULL) {
            //未定义对消息事件的处理
            tmos_msg_deallocate(pMsg); //释放消息
        }

        return events ^ SYS_EVENT_MSG;
    }

    if(events & SENSOR_TRIGGER_EVENT) //传感器更新触发事件
    {
        // LED2_ON();
        /* 触发传感器更新 */
        sensor_trigger();

        return events ^ SENSOR_TRIGGER_EVENT;
    }

    if(events & SENSOR_FINISH_EVENT) //传感器转换完成事件
    {
        /* 一定周期后执行下一次传感器更新操作 */
        tmos_start_task(sensor_task_id, SENSOR_TRIGGER_EVENT, MS1_TO_SYSTEM_TIME(SENSOR_UPDATE_PERIOD*1000));
        /* 更新传感器数据 */
        sensor_update();
        // LED2_OFF();

        return events ^ SENSOR_FINISH_EVENT;
    }
    
    return 0; //不处理未知事件
}

void sensor_task_init()
{
    sensor_task_id = TMOS_ProcessEventRegister(sensor_event_handler);

    sht3x_init();
    tmos_set_event(sensor_task_id, SENSOR_TRIGGER_EVENT); //立即执行一次传感器数值更新操作
}

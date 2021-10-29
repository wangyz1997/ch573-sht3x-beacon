#ifndef __TASK_BROADCASTER_H
#define __TASK_BROADCASTER_H

#include "CH57x_common.h"
#include "CH57xBLE_LIB.h"

extern tmosTaskID broadcaster_task_id;

void broadcaster_task_init(void);

#endif

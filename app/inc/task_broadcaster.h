#ifndef __BROADCASTER_H
#define __BROADCASTER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "CH57x_common.h"

extern uint8_t broadcaster_task_id;

void broadcaster_task_init(void);

#ifdef __cplusplus
}
#endif

#endif

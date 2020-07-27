#ifndef __TASK_LIST_H__
#define __TASK_LIST_H__

#include "ak.h"
#include "message.h"

#include "app_data.h"

#include "console.h"
#include "task_debug.h"
#include "task_life.h"

enum {
	/* SYSTEM TASKS */
	AK_TASK_TIMER_ID,
	AK_TASK_CONSOLE_ID,

	/* APP TASKS */	
	GW_TASK_DEBUG_ID,
	GW_TASK_LIFE_ID,

	/* EOT task ID */
	AK_TASK_LIST_LEN,
};

extern ak_task_t task_list[];

#endif //__TASK_LIST_H__

#include "timer.h"

#include "task_list.h"

ak_task_t task_list[] = {
	/* SYSTEM TASKS */
	{	AK_TASK_TIMER_ID,				TASK_PRI_LEVEL_1,	timer_entry						,	&timer_mailbox						,	"timer service"			},
	{	AK_TASK_CONSOLE_ID,				TASK_PRI_LEVEL_8,	console_entry					,	&console_mailbox					,	"handle commands"		},
	
	/* APP TASKS */			
	{	GW_TASK_DEBUG_ID,				TASK_PRI_LEVEL_1,	gw_task_debug_entry				,	&gw_task_debug_mailbox				,	"task debug message"	},
	{	GW_TASK_LIFE_ID,				TASK_PRI_LEVEL_1,	gw_task_life_entry				,	&gw_task_life_mailbox				,	"task life"				},
};

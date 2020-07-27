#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <vector>

#include "app.h"
#include "app_data.h"
#include "app_dbg.h"

#include "task_list.h"
#include "task_debug.h"

q_msg_t gw_task_debug_mailbox;

void* gw_task_debug_entry(void*) {
	ak_msg_t* msg = AK_MSG_NULL;

	wait_all_tasks_started();

	APP_DBG("[STARTED] gw_task_debug_entry\n");

	while (1) {
		/* get messge */
		msg = ak_msg_rev(GW_TASK_DEBUG_ID);

		switch (msg->header->sig) {

		case GW_DEBUG_1: {
			APP_DBG("GW_DEBUG_1\n");			
			
		}
			break;

		default:
			break;
		}

		/* free message */
		ak_msg_free(msg);
	}

	return (void*)0;
}

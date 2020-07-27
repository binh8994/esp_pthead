#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <vector>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"

#include "app.h"
#include "app_data.h"
#include "app_dbg.h"

#include "task_list.h"
#include "task_debug.h"

#define GPIO_OUTPUT_IO_1    16
#define GPIO_OUTPUT_PIN_SEL  (1ULL<<GPIO_OUTPUT_IO_1)

q_msg_t gw_task_life_mailbox;

void* gw_task_life_entry(void*) {
	ak_msg_t* msg = AK_MSG_NULL;

	wait_all_tasks_started();

	APP_DBG("[STARTED] gw_task_life_entry\n");

    gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO15/16
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    //disable pull-up mode
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    //configure GPIO with the given settings
    gpio_config(&io_conf);

    int cnt = 0;

    timer_set(GW_TASK_LIFE_ID, GW_BLINK_LED, 1000, TIMER_PERIODIC);

	while (1) {
		/* get messge */
		msg = ak_msg_rev(GW_TASK_LIFE_ID);

		switch (msg->header->sig) {

		case GW_BLINK_LED: {
			// APP_DBG_SIG("GW_BLINK_LED\n");	

            vTaskDelay(1000 / portTICK_RATE_MS);
            gpio_set_level((gpio_num_t)GPIO_OUTPUT_IO_1, cnt++ % 2);		
			
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

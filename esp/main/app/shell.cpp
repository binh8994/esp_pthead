#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linenoise/linenoise.h"
#include "argtable3/argtable3.h"

#include "esp_log.h"
#include "esp_console.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"

#include "ak.h"
#include "timer.h"

#include "app.h"
#include "app_dbg.h"
#include "shell.h"

#include "task_list.h"

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    /* For accessing reason codes in case of disconnection */
    system_event_info_t *info = &event->event_info;

    switch(event->event_id) {
    case SYSTEM_EVENT_STA_GOT_IP:
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        ESP_LOGI(__func__, "Disconnect reason : %d", info->disconnected.reason);
        if (info->disconnected.reason == WIFI_REASON_BASIC_RATE_NOT_SUPPORT) {
            /*Switch to 802.11 bgn mode */
            esp_wifi_set_protocol(ESP_IF_WIFI_STA, WIFI_PROTOCAL_11B | WIFI_PROTOCAL_11G | WIFI_PROTOCAL_11N);
        }
        esp_wifi_connect();
        break;
    default:
        break;
    }
    return ESP_OK;
}

static void initialise_wifi(void)
{
    esp_log_level_set("wifi", ESP_LOG_WARN);
    static bool initialized = false;
    if (initialized) {
        return;
    }
    tcpip_adapter_init();
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_NULL) );
    ESP_ERROR_CHECK( esp_wifi_start() );
    initialized = true;
}

static void wifi_join(const char* ssid, const char* pass, int timeout_ms)
{
    initialise_wifi();
    wifi_config_t wifi_config;
    strncpy((char*) wifi_config.sta.ssid, ssid, sizeof(wifi_config.sta.ssid));
    if (pass) {
        strncpy((char*) wifi_config.sta.password, pass, sizeof(wifi_config.sta.password));
    }

    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    ESP_ERROR_CHECK( esp_wifi_connect() );
}

/** Arguments used by 'join' function */
static struct {
    struct arg_int *timeout;
    struct arg_str *ssid;
    struct arg_str *password;
    struct arg_end *end;
} join_args;

static int connect(int argc, char** argv)
{
    int nerrors = arg_parse(argc, argv, (void**) &join_args);
    if (nerrors != 0) {
        arg_print_errors(stderr, join_args.end, argv[0]);
        return -1;
    }
    ESP_LOGI(__func__, "Connecting to '%s'", join_args.ssid->sval[0]);

    wifi_join(join_args.ssid->sval[0], join_args.password->sval[0], join_args.timeout->ival[0]);
    
    return 0;
}

static void register_wifi()
{
    join_args.timeout = arg_int0(NULL, "timeout", "<t>", "Connection timeout, ms");
    join_args.timeout->ival[0] = 5000; // set default value
    join_args.ssid = arg_str1(NULL, NULL, "<ssid>", "SSID of AP");
    join_args.password = arg_str0(NULL, NULL, "<pass>", "PSK of AP");
    join_args.end = arg_end(2);

    const esp_console_cmd_t join_cmd = {
        .command = "join",
        .help = "Join WiFi AP as a station",
        .hint = NULL,
        .func = &connect,
        .argtable = &join_args
    };

    ESP_ERROR_CHECK( esp_console_cmd_register(&join_cmd) );
}

static struct {    
    struct arg_str *type;
    struct arg_end *end;
} dbg_args;

static int dbg_func(int argc, char** argv){
    if (argc != 2) {
        ESP_LOGI(__func__, "cmd error");
        return 1;
    }

    if (strcmp("pur", argv[1]) == 0) {
        task_post_pure_msg(GW_TASK_DEBUG_ID, GW_DEBUG_1);
    }
    else if (strcmp("com", argv[1]) == 0) {
        uint8_t data[64];
        task_post_common_msg(GW_TASK_DEBUG_ID, GW_DEBUG_1, (uint8_t*)data, 64);
    }
    else if (strcmp("dyn", argv[1]) == 0) {
        uint8_t data[64];
        task_post_dynamic_msg(GW_TASK_DEBUG_ID, GW_DEBUG_1, (uint8_t*)data, 64);
    }
    else if (strcmp("tim", argv[1]) == 0) {
        timer_set(GW_TASK_DEBUG_ID, GW_DEBUG_1, 3000, TIMER_ONE_SHOT);
    }
    else {
        ESP_LOGI(__func__, "argv unknown");
    }

    return 0;
}

static void register_dbg() {

    dbg_args.type = arg_str0(NULL, NULL, "<argv>", "Argv 0, string");
    dbg_args.end = arg_end(1);

    const esp_console_cmd_t dbg_cmd = {
        .command = "dbg",
        .help = "Debug console",
        .hint = NULL,
        .func = &dbg_func,
        .argtable = &dbg_args
    };

    ESP_ERROR_CHECK( esp_console_cmd_register(&dbg_cmd) );
}

void register_cmd() {
	esp_console_register_help_command();
	register_wifi();
    register_dbg();
}
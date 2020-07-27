#include "nvs.h"
#include "nvs_flash.h"

#include "ak.h"

#include "app.h"
#include "app_dbg.h"

static void initialize_nvs()
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
        ESP_ERROR_CHECK( nvs_flash_erase() );
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

void task_init() {

    initialize_nvs();

}

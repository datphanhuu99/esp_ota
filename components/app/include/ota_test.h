#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "esp_ota_ops.h"
#include "esp_http_client.h"
#include "esp_https_ota.h"
#include "esp_crt_bundle.h"


static const char *TAG = "OTA";
// #define WIFI_SSID      CONFIG_WIFI_SSID
// #define WIFI_PASSWORD  CONFIG_WIFI_PASSWORD
#define OTA_URL        CONFIG_OTA_UPDATE_URL

void ota_update(void *arg);
#include <stdio.h>

#include "gpio.h"
#include "touch.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

static SemaphoreHandle_t sync_touch_task;
static SemaphoreHandle_t sync_stats_task;
static SemaphoreHandle_t sync_wifi_task;

static void app_read_touch(void *arg);
static void app_init(void);

static esp_err_t print_real_time_stats(TickType_t xTicksToWait);
static void app_stats_task(void *arg);
static void app_ota_update(void *arg);

void app_main(void);


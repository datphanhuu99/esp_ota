#include "main.h"
#include <stdio.h>
#include <stdlib.h>

#include "freertos/semphr.h"    
#include "esp_err.h"

#include "gpio.h"
#include "touch.h"
#include "wifi.h"
#include "led.h"
#include <ota_test.h>

#include "nvs_flash.h"


#define NUM_OF_SPIN_TASKS 4
#define SPIN_ITER 5000000
#define TOUCH_TASK_PRIO 2
#define STATS_TASK_PRIO 3
#define STATS_TICK pdMS_TO_TICKS(1000)
#define ARRAY_SIZE_OFFSET 10

static bool wifi_is_ready = false;

static void app_init(void)
{
    touch_init_val();
    gpio_init_driver();
}
// read touch data and send control the led-
static void app_read_touch(void *arg)
{   
    xSemaphoreTake(sync_touch_task, portMAX_DELAY);
    printf("app_read_touch\n");
    uint16_t val = 0;
    while(1){
        val = touch_read_val(0);
        printf("Touch value %d:[%4"PRIu16"] ", 0, val);
        if (val > 1000){
            printf("led on");

            gpio_set(2);
        } else {
            printf("led off");
            gpio_reset(2);
        }
        printf("\n");
        vTaskDelay(pdMS_TO_TICKS(100));
        xSemaphoreGive(sync_touch_task);
    }
}

static void app_wifi(void *arg)
{
    xSemaphoreTake(sync_wifi_task, portMAX_DELAY);
    printf("app_wifi\n");

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    if(wifi_init()>0){
        wifi_is_ready = true;
        printf("wifi_init success\n");
    }
    // wifi_set_mode();
    // wifi_connect();
    // wifi_scan();
    while (1)
    {
        printf("app_wifi task running\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
        xSemaphoreGive(sync_wifi_task);
    }
}

static void app_ota_update(void *arg)
{
    xSemaphoreTake(sync_wifi_task, portMAX_DELAY);
    printf("app_ota_update\n");
    while (1)
    {
        printf("app_ota_update wait wifi ready\n");
        if(wifi_is_ready){
            ota_update();
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(1000));

    }
}

static esp_err_t print_real_time_stats(TickType_t xTicksToWait)
{
    TaskStatus_t *start_array = NULL, *end_array = NULL;
    UBaseType_t start_array_size, end_array_size;
    configRUN_TIME_COUNTER_TYPE start_run_time, end_run_time;
    esp_err_t ret;

    //Allocate array to store current task states
    start_array_size = uxTaskGetNumberOfTasks() + ARRAY_SIZE_OFFSET;
    start_array = malloc(sizeof(TaskStatus_t) * start_array_size);
    if (start_array == NULL) {
        ret = ESP_ERR_NO_MEM;
        goto exit;
    }
    //Get current task states
    start_array_size = uxTaskGetSystemState(start_array, start_array_size, &start_run_time);
    if (start_array_size == 0) {
        ret = ESP_ERR_INVALID_SIZE;
        goto exit;
    }

    vTaskDelay(xTicksToWait);

    //Allocate array to store tasks states post delay
    end_array_size = uxTaskGetNumberOfTasks() + ARRAY_SIZE_OFFSET;
    end_array = malloc(sizeof(TaskStatus_t) * end_array_size);
    if (end_array == NULL) {
        ret = ESP_ERR_NO_MEM;
        goto exit;
    }
    //Get post delay task states
    end_array_size = uxTaskGetSystemState(end_array, end_array_size, &end_run_time);
    if (end_array_size == 0) {
        ret = ESP_ERR_INVALID_SIZE;
        goto exit;
    }

    //Calculate total_elapsed_time in units of run time stats clock period.
    uint32_t total_elapsed_time = (end_run_time - start_run_time);
    if (total_elapsed_time == 0) {
        ret = ESP_ERR_INVALID_STATE;
        goto exit;
    }

    printf("| Task | Run Time | Percentage\n");
    //Match each task in start_array to those in the end_array
    for (int i = 0; i < start_array_size; i++) {
        int k = -1;
        for (int j = 0; j < end_array_size; j++) {
            if (start_array[i].xHandle == end_array[j].xHandle) {
                k = j;
                //Mark that task have been matched by overwriting their handles
                start_array[i].xHandle = NULL;
                end_array[j].xHandle = NULL;
                break;
            }
        }
        //Check if matching task found
        if (k >= 0) {
            uint32_t task_elapsed_time = end_array[k].ulRunTimeCounter - start_array[i].ulRunTimeCounter;
            uint32_t percentage_time = (task_elapsed_time * 100UL) / (total_elapsed_time * CONFIG_FREERTOS_NUMBER_OF_CORES);
            printf("| %s | %"PRIu32" | %"PRIu32"%%\n", start_array[i].pcTaskName, task_elapsed_time, percentage_time);
        }
    }

    //Print unmatched tasks
    for (int i = 0; i < start_array_size; i++) {
        if (start_array[i].xHandle != NULL) {
            printf("| %s | Deleted\n", start_array[i].pcTaskName);
        }
    }
    for (int i = 0; i < end_array_size; i++) {
        if (end_array[i].xHandle != NULL) {
            printf("| %s | Created\n", end_array[i].pcTaskName);
        }
    }
    ret = ESP_OK;

exit:    //Common return path
    free(start_array);
    free(end_array);
    return ret;
}

static void app_stats_task(void *arg)
{
    xSemaphoreTake(sync_stats_task, portMAX_DELAY);

    xSemaphoreGive(sync_touch_task);
    xSemaphoreGive(sync_wifi_task);

    while(1)
    {
        printf("Getting real time stats over %"PRIu32" tick \n", STATS_TICK);
        if(print_real_time_stats(STATS_TICK) == ESP_OK){
            printf("Real time stats printed successfully\n");
        } else {
            printf("Failed to print real time stats\n");
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}


void app_main(void){
    app_init();
    printf("app_main\n");
    // create a task to read touch data
    // and control the led

    sync_stats_task = xSemaphoreCreateBinary();
    sync_touch_task = xSemaphoreCreateBinary();
    sync_wifi_task = xSemaphoreCreateBinary();


    xTaskCreatePinnedToCore(app_read_touch, "app_read_touch", 4096, NULL, TOUCH_TASK_PRIO, NULL,tskNO_AFFINITY);
    xTaskCreatePinnedToCore(app_stats_task, "app_stats_task", 4096, NULL, STATS_TASK_PRIO, NULL,tskNO_AFFINITY);
    xTaskCreatePinnedToCore(app_wifi, "app_wifi", 4096, NULL, TOUCH_TASK_PRIO, NULL,tskNO_AFFINITY);
    xTaskCreatePinnedToCore(app_ota_update, "app_ota_update", 8192, NULL, 1, NULL,tskNO_AFFINITY);
    xSemaphoreGive(sync_stats_task);
    // uint16_t val = 0;
    // while(1){
    //     val = touch_read(0);
    //     printf("Touch value %d", val);
    //     if (val > 1000){
    //         gpio_set(2);
    //     } else {
    //         gpio_reset(2);
    //     }
    //     printf("\n");
    //     vTaskDelay(200 / portTICK_PERIOD_MS);
    // }
    return;
}
#include <stdio.h>
#include <driver/gpio.h>
#include "driver/touch_pad.h"
#include "driver/touch_sensor.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"


void app_main(void)
{
    gpio_set_direction(GPIO_NUM_2,GPIO_MODE_OUTPUT);
    ESP_ERROR_CHECK(touch_pad_init());
    touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);
    touch_pad_config(0, 0);
    while(1){

        uint16_t val =0;
        touch_pad_read(0,&val);
        printf("T%d:[%4"PRIu16"] ", 0, val);

        if (val>1000){
            gpio_set_level(GPIO_NUM_2,1);
        } else {
            gpio_set_level(GPIO_NUM_2,0);
        }
        
    }
}

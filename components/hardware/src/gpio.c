#include "gpio.h"

static gpio_config_t io_conf = {
    .intr_type = GPIO_INTR_DISABLE,
    .mode = GPIO_MODE_OUTPUT,
    .pin_bit_mask = (1ULL<<GPIO_NUM_2),
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .pull_up_en = GPIO_PULLUP_ENABLE,
};

void gpio_init_driver(){
    gpio_config(&io_conf);
}

bool gpio_is_set(int16_t pin){
    return 1==gpio_get_level(pin);
}
void gpio_set(int16_t pin){
    // if(gpio_get_level(pin)==1){
    //     printf("GPIO %d already set\n", pin);
    //     return;
    // }
    if(gpio_set_level(pin, 1)!=ESP_OK){
        printf("Failed to set GPIO %d\n", pin);
    }
}
void gpio_reset(int16_t pin){
        // if(gpio_get_level(pin)==0){
        //     printf("GPIO %d already reset\n", pin);
        //     return;
        // }
    if (gpio_set_level(pin, 0)!=ESP_OK){
        printf("Failed to reset GPIO %d\n", pin);
    }
}

int16_t gpio_read_pin(int16_t pin){
    return gpio_get_level(pin);
}

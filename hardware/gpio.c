#include "gpio.h"

void gpio_init_driver(){
    gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
}

void gpio_set(int16_t pin){
    gpio_set_level(pin, 1);
}

void gpio_reset(int16_t pin){
    gpio_set_level(pin, 0);
}

int16_t gpio_read_pin(int16_t pin){
    return gpio_get_level(pin);
}

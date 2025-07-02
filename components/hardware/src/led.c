#include "led.h"

void led_init()
{
    // Initialize the LED GPIO pin
    // gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
}

int led_test(int pos)
{
    int val = 0;
    if (pos == 0) {
        val = 1100;
    }
    return val;
}
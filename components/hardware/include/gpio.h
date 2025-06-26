#include "driver/gpio.h"

void gpio_init_driver();
void gpio_deinit();
void gpio_set(int16_t pin);
void gpio_reset(int16_t pin);
int16_t gpio_read_pin(int16_t pin);
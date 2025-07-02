#include "touch.h"



void touch_init_val(void)
{
    // Initialize touch pad peripheral.
    ESP_ERROR_CHECK(touch_pad_init());
    // Set reference voltage for charging/discharging
    touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);
    // Configure touch pad 0
    touch_pad_config(0, 0);
    // start touch pad filter 
    touch_pad_filter_start(10);
}
uint16_t touch_read_val(uint16_t touch_pos)
{
    uint16_t val = 0;
    // read touch pad value
    touch_pad_read(touch_pos, &val);
    return val;
}
void touch_deinit(void)
{
    touch_pad_deinit();
}
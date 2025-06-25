idf_component_register(SRCS "app/main.c" "app/gpio.c" "app/led.c"  "app/touch.c" "app/wifi.c"
                    INCLUDE_DIRS "." "app"
                    REQUIRES esp_driver_gpio driver esp_wifi nvs_flash)

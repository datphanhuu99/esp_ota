idf_component_register(SRCS "src/gpio.c"
                            "src/led.c"
                            "src/touch.c"
                            "src/wifi.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_driver_gpio driver esp_wifi nvs_flash)
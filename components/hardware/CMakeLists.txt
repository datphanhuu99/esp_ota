idf_component_register(SRCS "src/gpio.c"
                            "src/led.c"
                            "src/touch.c"
                            "src/wifi.c"
                        #     "../app/src/ota_test.c"
                    INCLUDE_DIRS "include" 
                    REQUIRES esp_driver_gpio driver esp_wifi nvs_flash)

# idf_component_register(SRCS "src/gpio.c"
#                             "src/led.c"
#                             "src/touch.c"
#                             "src/wifi.c"
#                             "src/ota.c"
#                     INCLUDE_DIRS "include"
#                     REQUIRES esp_event esp_netif esp_wifi nvs_flash esp_https_ota esp_http_client app_update mbedtls
#                     EMBED_TXTFILES ${project_dir}/server_certs/ca_cert.pem)

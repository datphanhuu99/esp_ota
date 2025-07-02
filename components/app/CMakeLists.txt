idf_component_register(SRCS "src/ota_test.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_event esp_netif esp_wifi nvs_flash esp_https_ota esp_http_client app_update mbedtls
                    EMBED_TXTFILES ${project_dir}/server_certs/ca_cert.pem
                    )

# idf_component_register(SRCS 
#                             "../app/src/ota_test.c"
#                     INCLUDE_DIRS "include" 
#                     REQUIRES esp_driver_gpio driver esp_wifi nvs_flash )
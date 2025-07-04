idf_component_register(SRCS "main.c" 
                    INCLUDE_DIRS "."
                    REQUIRES esp_driver_gpio driver esp_wifi nvs_flash hardware esp_https_ota app
                    # EMBED_TXTFILES ${project_dir}/server_certs/ca_cert.pem
                    )

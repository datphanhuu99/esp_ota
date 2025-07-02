#include "ota_test.h"

extern const uint8_t server_cert_pem_start[] asm("_binary_server_cert_pem_start");
extern const uint8_t server_cert_pem_end[]   asm("_binary_server_cert_pem_end");

esp_err_t _http_event_handler(esp_http_client_event_t *evt){
    switch (evt->event_id) {
    case HTTP_EVENT_ERROR:
        ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
        break;
    case HTTP_EVENT_ON_CONNECTED:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
        break;
    case HTTP_EVENT_HEADER_SENT:
        ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
        break;
    case HTTP_EVENT_ON_HEADER:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
        break;
    case HTTP_EVENT_ON_DATA:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
        break;
    case HTTP_EVENT_ON_FINISH:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
        break;
    case HTTP_EVENT_DISCONNECTED:
        ESP_LOGD(TAG, "HTTP_EVENT_DISCONNECTED");
        break;
    case HTTP_EVENT_REDIRECT:
        ESP_LOGD(TAG, "HTTP_EVENT_REDIRECT");
        break;
    }
    return ESP_OK;
}

void ota_update(void *arg)
{
    ESP_LOGI(TAG, "Starting OTA update from %s", OTA_URL);
    const esp_http_client_config_t config = {
        .url = OTA_URL,
        .crt_bundle_attach= esp_crt_bundle_attach, // Attach the certificate bundle for HTTPS
        // .cert_pem = NULL, // Add your certificate here if needed
        .event_handler = _http_event_handler,
        .keep_alive_enable = true,
        // .tls_dyn_buf_strategy = HTTP_TLS_DYN_BUF_RX_STATIC,
        .timeout_ms = 10000,
        .skip_cert_common_name_check = true, // Skip common name check for simplicity
    };

    ESP_LOGI(TAG, "Attempting to download update from %s", config.url);

    esp_https_ota_config_t ota_config = {
        .http_config = &config,
    };


    // Hàm esp_https_ota xử lý toàn bộ quá trình:
    // 1. Tải firmware từ URL
    // 2. Ghi vào phân vùng OTA không hoạt động
    // 3. Xác thực firmware
    esp_err_t ret = esp_https_ota(&ota_config);
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "OTA Update successful! Rebooting...");
        esp_restart();
    } else {
        ESP_LOGE(TAG, "Firmware upgrade failed. Error: %s", esp_err_to_name(ret));
    }

    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    
    // Tự hủy task khi xong việc (kể cả khi lỗi)
    vTaskDelete(NULL);

}
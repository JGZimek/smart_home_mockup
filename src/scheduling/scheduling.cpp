#include "scheduling.hpp"

#define SCHEDULING_TAG "app_scheduling"

TaskHandle_t wifiTaskHandle = NULL;
TaskHandle_t rfidTaskHandle = NULL;

bool esp_setup()
{
    if (!init_wifi())
    {
        ESP_LOGE(SCHEDULING_TAG, "Failed to initialize WiFi");
        return false;
    }

    if (!init_RFID())
    {
        ESP_LOGE(SCHEDULING_TAG, "Failed to initialize RFID");
        return false;
    }

    if (!init_scheduling())
    {
        ESP_LOGE(SCHEDULING_TAG, "Scheduling initialization failed. System halted.");
        return false;
    }

    return true;
}

void wifiTask(void *pvParameters)
{
    while (1)
    {
        handle_wifi();
        vTaskDelay(WIFI_RECONNECT_FREQ / portTICK_PERIOD_MS);
    }
}

void rfidTask(void *pvParameters)
{
    while (1)
    {
        handle_RFID();
        vTaskDelay(RFID_READ_FREQ / portTICK_PERIOD_MS);
    }
}

bool init_scheduling()
{
    ESP_LOGI(SCHEDULING_TAG, "Initializing scheduling...");

    BaseType_t result;

    result = xTaskCreatePinnedToCore(
        wifiTask,
        "Wifi Task",
        WIFI_TASK_STACK_SIZE,
        NULL,
        WIFI_TASK_PRIORITY,
        &wifiTaskHandle,
        WIFI_CORE);

    if (result != pdPASS)
    {
        ESP_LOGE(SCHEDULING_TAG, "Failed to create Network Task");
        return false;
    }

    result = xTaskCreatePinnedToCore(
        rfidTask,
        "RFID Task",
        RFID_TASK_STACK_SIZE,
        NULL,
        RFID_TASK_PRIORITY,
        &rfidTaskHandle,
        RFID_CORE);

    if (result != pdPASS)
    {
        ESP_LOGE(SCHEDULING_TAG, "Failed to create RFID Task");
        return false;
    }

    ESP_LOGI(SCHEDULING_TAG, "Scheduling initialized successfully.");
    return true;
}

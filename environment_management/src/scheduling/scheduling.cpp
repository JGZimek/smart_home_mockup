#include "scheduling.hpp"
#include "esp_log.h"

#define SCHEDULING_TAG "app_scheduling"

TaskHandle_t wifi_task;
TaskHandle_t mqtt_task;

WiFiManager wifiManager;
MqttManager mqttManager;

bool esp_setup()
{
    ESP_LOGI(SCHEDULING_TAG, "Setting up ESP32 system...");

    // Check if both WiFi and MQTT credentials are configured.
    if (!(wifiManager.isConfigured() && mqttManager.isConfigured()))
    {
        ESP_LOGI(SCHEDULING_TAG, "WiFi and/or MQTT credentials not configured. Starting Access Point mode...");
        // Launch AP mode (this call blocks until valid settings are entered)
        AccessPoint ap("ESP32_Config", "config123");
        ap.run();
    }

    // Attempt to initialize WiFi
    if (!wifiManager.begin())
    {
        ESP_LOGE(SCHEDULING_TAG, "WiFiManager setup failed.");
        return false;
    }

    // Attempt to initialize MQTT connection (which loads its configuration from Preferences)
    if (!mqttManager.begin())
    {
        ESP_LOGE(SCHEDULING_TAG, "MQTT Manager setup failed.");
        return false;
    }

    return true;
}

bool init_scheduling()
{
    ESP_LOGI(SCHEDULING_TAG, "Initializing scheduling...");

    BaseType_t result;

    result = xTaskCreatePinnedToCore(
        wifiTask,
        "wifi_task",
        WIFI_TASK_STACK_SIZE,
        NULL,
        WIFI_TASK_PRIORITY,
        &wifi_task,
        WIFI_TASK_CORE);
    if (result != pdPASS)
    {
        ESP_LOGE(SCHEDULING_TAG, "Failed to create wifi task.");
        return false;
    }

    result = xTaskCreatePinnedToCore(
        mqttTask,
        "mqtt_task",
        MQTT_TASK_STACK_SIZE,
        NULL,
        MQTT_TASK_PRIORITY,
        &mqtt_task,
        MQTT_TASK_CORE);
    if (result != pdPASS)
    {
        ESP_LOGE(SCHEDULING_TAG, "Failed to create mqtt task.");
        return false;
    }

    return true;
}

void wifiTask(void *pvParameters)
{
    while (true)
    {
        wifiManager.handle();
        vTaskDelay(WIFI_EVENT_FREQUENCY / portTICK_PERIOD_MS);
    }
}

void mqttTask(void *pvParameters)
{
    while (true)
    {
        mqttManager.handle();
        vTaskDelay(MQTT_EVENT_FREQUENCY / portTICK_PERIOD_MS);
    }
}

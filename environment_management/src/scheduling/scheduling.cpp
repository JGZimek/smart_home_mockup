#include "scheduling.hpp"

#define SCHEDULING_TAG "app_scheduling"

TaskHandle_t wifi_task;
WiFiManager wifiManager;

bool esp_setup()
{
    ESP_LOGI(SCHEDULING_TAG, "Setting up ESP32 system...");

    if (!wifiManager.isConfigured())
    {
        ESP_LOGI(SCHEDULING_TAG, "WiFi credentials not configured. Starting Access Point mode...");

        // Tworzymy instancję AccessPoint – podajemy dowolne AP SSID oraz hasło
        // run() blokuje wykonywanie do momentu, aż użytkownik zapisze dane w Preferences
        AccessPoint ap("ESP32_Config", "config123");
        ap.run();
    }

    if (!wifiManager.begin())
    {
        ESP_LOGE(SCHEDULING_TAG, "WiFiManager setup failed.");
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

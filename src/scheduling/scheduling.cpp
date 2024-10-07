#include "scheduling.hpp"

#define SCHEDULING_TAG "app_scheduling"

TaskHandle_t networkTaskHandle = NULL;
TaskHandle_t communicationTaskHandle = NULL;

WiFiClient wifiClient;
PubSubClient mqttClient;
mqtt_topics topics("home", "security");

bool esp_setup()
{
    if (!initWiFi())
    {
        ESP_LOGE(SCHEDULING_TAG, "Failed to initialize WiFi");
        return false;
    }

    if (!initMQTT(mqttClient, wifiClient, topics))
    {
        ESP_LOGE(SCHEDULING_TAG, "Failed to initialize MQTT");
        return false;
    }

    if (!initScheduling())
    {
        ESP_LOGE(SCHEDULING_TAG, "Scheduling initialization failed. System halted.");
        return false;
    }

    return true;
}

void networkTask(void *pvParameters)
{
    while (1)
    {
        handleWiFi();
        vTaskDelay(NETWORK_RECONNECT_FREQ / portTICK_PERIOD_MS);
    }
}

void communicationTask(void *pvParameters)
{
    while (1)
    {
        handleMQTT(mqttClient, topics);
        vTaskDelay(COMMUNICATION_EVENT_FREQ / portTICK_PERIOD_MS);
    }
}

bool initScheduling()
{
    ESP_LOGI(SCHEDULING_TAG, "Initializing scheduling...");

    BaseType_t result;

    result = xTaskCreatePinnedToCore(networkTask, "Network Task", NETWORK_TASK_STACK_SIZE, NULL, NETWORK_TASK_PRIORITY, &networkTaskHandle, NETWORK_CORE);
    if (result != pdPASS)
    {
        ESP_LOGE(SCHEDULING_TAG, "Failed to create Network Task");
        return false;
    }

    result = xTaskCreatePinnedToCore(communicationTask, "Communication Task", COMMUNICATION_TASK_STACK_SIZE, NULL, COMMUNICATION_TASK_PRIORITY, &communicationTaskHandle, COMMUNICATION_CORE);
    if (result != pdPASS)
    {
        ESP_LOGE(SCHEDULING_TAG, "Failed to create Communication Task");
        return false;
    }

    ESP_LOGI(SCHEDULING_TAG, "Scheduling initialized successfully.");
    return true;
}

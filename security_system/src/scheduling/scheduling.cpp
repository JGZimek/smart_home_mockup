#include "scheduling.hpp"

#define SCHEDULING_TAG "app_scheduling"

TaskHandle_t wifiTaskHandle = NULL;
TaskHandle_t rfidTaskHandle = NULL;
TaskHandle_t pinpadTaskHandle = NULL;
TaskHandle_t buzzerTaskHandle = NULL;
TaskHandle_t pirTaskHandle = NULL;
TaskHandle_t mqttTaskHandle = NULL;

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

    if (!init_pinpad())
    {
        ESP_LOGE(SCHEDULING_TAG, "Failed to initialize Pinpad");
        return false;
    }

    if (!init_buzzer())
    {
        ESP_LOGE(SCHEDULING_TAG, "Failed to initialize Buzzer");
        return false;
    }

    if (!init_pir())
    {
        ESP_LOGE(SCHEDULING_TAG, "Failed to initialize PIR");
        return false;
    }

    if (!init_mqtt())
    {
        ESP_LOGE(SCHEDULING_TAG, "Failed to initialize MQTT");
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

void pinpadTask(void *pvParameters)
{
    while (1)
    {
        handle_pinpad();
        vTaskDelay(PINPAD_READ_FREQ / portTICK_PERIOD_MS);
    }
}

void buzzerTask(void *pvParameters)
{
    while (1)
    {
        handle_buzzer();
        vTaskDelay(BUZZER_READ_FREQ / portTICK_PERIOD_MS);
    }
}

void pirTask(void *pvParameters)
{
    while (1)
    {
        handle_pir();
        vTaskDelay(PIR_READ_FREQ / portTICK_PERIOD_MS);
    }
}

void mqttTask(void *pvParameters)
{
    while (1)
    {
        handle_mqtt();
        vTaskDelay(MQTT_READ_FREQ / portTICK_PERIOD_MS);
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

    result = xTaskCreatePinnedToCore(
        pinpadTask,
        "Pinpad Task",
        PINPAD_TASK_STACK_SIZE,
        NULL,
        PINPAD_TASK_PRIORITY,
        &pinpadTaskHandle,
        PINPAD_CORE);

    if (result != pdPASS)
    {
        ESP_LOGE(SCHEDULING_TAG, "Failed to create Pinpad Task");
        return false;
    }

    result = xTaskCreatePinnedToCore(
        buzzerTask,
        "Buzzer Task",
        BUZZER_TASK_STACK_SIZE,
        NULL,
        BUZZER_TASK_PRIORITY,
        &buzzerTaskHandle,
        BUZZER_CORE);

    if (result != pdPASS)
    {
        ESP_LOGE(SCHEDULING_TAG, "Failed to create Buzzer Task");
        return false;
    }

    result = xTaskCreatePinnedToCore(
        pirTask,
        "PIR Task",
        PIR_TASK_STACK_SIZE,
        NULL,
        PIR_TASK_PRIORITY,
        &pirTaskHandle,
        PIR_CORE);

    if (result != pdPASS)
    {
        ESP_LOGE(SCHEDULING_TAG, "Failed to create PIR Task");
        return false;
    }

    result = xTaskCreatePinnedToCore(
        mqttTask,
        "MQTT Task",
        MQTT_TASK_STACK_SIZE,
        NULL,
        MQTT_TASK_PRIORITY,
        &mqttTaskHandle,
        MQTT_CORE);

    if (result != pdPASS)
    {
        ESP_LOGE(SCHEDULING_TAG, "Failed to create MQTT Task");
        return false;
    }

    ESP_LOGI(SCHEDULING_TAG, "Scheduling initialized successfully.");
    return true;
}

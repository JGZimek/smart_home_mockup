#include "scheduling.hpp"

#define SCHEDULING_TAG "app_scheduling"

TaskHandle_t wifi_task;
TaskHandle_t mqtt_task;
TaskHandle_t fan_control_task;
TaskHandle_t env_measurement_task;
TaskHandle_t button_task;
TaskHandle_t led_control_task;

WiFiManager wifiManager;
MqttManager mqttManager;
Button button;

bool esp_setup()
{
    ESP_LOGI(SCHEDULING_TAG, "Setting up ESP32 system...");

    if (!button.begin())
    {
        ESP_LOGE(SCHEDULING_TAG, "Button initialization failed.");
        return false;
    }

    wifiManager.begin();
    mqttManager.begin();

    if (!init_fan_control())
    {
        ESP_LOGE(SCHEDULING_TAG, "Fan control initialization failed.");
        return false;
    }

    if (!init_env_measurement())
    {
        ESP_LOGE(SCHEDULING_TAG, "Environmental measurement initialization failed.");
        return false;
    }

    if (!init_led_control())
    {
        ESP_LOGE(SCHEDULING_TAG, "LED control initialization failed.");
        return false;
    }

    if (!init_scheduling())
    {
        ESP_LOGE(SCHEDULING_TAG, "Scheduling initialization failed.");
        return false;
    }

    return true;
}

bool init_scheduling()
{
    ESP_LOGI(SCHEDULING_TAG, "Initializing scheduling...");

    BaseType_t result;

    result = xTaskCreatePinnedToCore(
        buttonTask,
        "button_task",
        BUTTON_TASK_STACK_SIZE,
        NULL,
        BUTTON_TASK_PRIORITY,
        &button_task,
        BUTTON_TASK_CORE);

    if (result != pdPASS)
    {
        ESP_LOGE(SCHEDULING_TAG, "Failed to create button task.");
        return false;
    }

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

    result = xTaskCreatePinnedToCore(
        fanControlTask,
        "fan_control_task",
        FAN_CONTROL_TASK_STACK_SIZE,
        NULL,
        FAN_CONTROL_TASK_PRIORITY,
        &fan_control_task,
        FAN_CONTROL_TASK_CORE);
    if (result != pdPASS)
    {
        ESP_LOGE(SCHEDULING_TAG, "Failed to create fan control task.");
        return false;
    }

    result = xTaskCreatePinnedToCore(
        envMeasurementTask,
        "env_measurement_task",
        ENV_MEASUREMENT_TASK_STACK_SIZE,
        NULL,
        ENV_MEASUREMENT_TASK_PRIORITY,
        &env_measurement_task,
        ENV_MEASUREMENT_TASK_CORE);
    if (result != pdPASS)
    {
        ESP_LOGE(SCHEDULING_TAG, "Failed to create environmental measurement task.");
        return false;
    }

    result = xTaskCreatePinnedToCore(
        ledControlTask,
        "led_control_task",
        LED_CONTROL_TASK_STACK_SIZE,
        NULL,
        LED_CONTROL_TASK_PRIORITY,
        &led_control_task,
        LED_CONTROL_TASK_CORE);
    if (result != pdPASS)
    {
        ESP_LOGE(SCHEDULING_TAG, "Failed to create led control task.");
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

void buttonTask(void *pvParameters)
{
    while (true)
    {
        button.handle();
        vTaskDelay(BUTTON_EVENT_FREQUENCY / portTICK_PERIOD_MS);
    }
}

void fanControlTask(void *pvParameters)
{
    while (true)
    {
        handle_fan_control();
        vTaskDelay(FAN_CONTROL_EVENT_FREQUENCY / portTICK_PERIOD_MS);
    }
}

void envMeasurementTask(void *pvParameters)
{
    while (true)
    {
        handle_env_measurement();
        vTaskDelay(ENV_MEASUREMENT_EVENT_FREQUENCY / portTICK_PERIOD_MS);
    }
}

void ledControlTask(void *pvParameters)
{
    while (true)
    {
        handle_led_control();
        vTaskDelay(LED_CONTROL_EVENT_FREQUENCY / portTICK_PERIOD_MS);
    }
}
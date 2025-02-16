#include "scheduling.hpp"

#define SCHEDULING_TAG "app_scheduling"

TaskHandle_t energy_monitor_task;

bool esp_setup()
{
    ESP_LOGI(SCHEDULING_TAG, "Setting up ESP32 system...");

    if (!init_energy_monitor())
    {
        ESP_LOGE(SCHEDULING_TAG, "Energy monitor initialization failed.");
        return false;
    }

    if (!init_led_control())
    {
        ESP_LOGE(SCHEDULING_TAG, "LED control initialization failed.");
        return false;
    }

    if (!init_light_intensity())
    {
        ESP_LOGE(SCHEDULING_TAG, "Light intensity initialization failed.");
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
        energymonitorTask,
        "energy_monitor",
        ENERGY_MONITOR_STACK_SIZE,
        NULL,
        ENERGY_MONITOR_PRIORITY,
        &energy_monitor_task,
        ENERGY_MONITOR_CORE);

    if (result != pdPASS)
    {
        ESP_LOGE(SCHEDULING_TAG, "Failed to create energy_monitor task.");
        return false;
    }

    result = xTaskCreatePinnedToCore(
        ledcontrolTask,
        "led_control",
        LED_CONTROL_STACK_SIZE,
        NULL,
        LED_CONTROL_PRIORITY,
        NULL,
        LED_CONTROL_CORE);

    if (result != pdPASS)
    {
        ESP_LOGE(SCHEDULING_TAG, "Failed to create led_control task.");
        return false;
    }

    result = xTaskCreatePinnedToCore(
        lightintensityTask,
        "light_intensity",
        LIGHT_INTENSITY_STACK_SIZE,
        NULL,
        LIGHT_INTENSITY_PRIORITY,
        NULL,
        LIGHT_INTENSITY_CORE);

    if (result != pdPASS)
    {
        ESP_LOGE(SCHEDULING_TAG, "Failed to create light_intensity task.");
        return false;
    }

    return true;
}

void energymonitorTask(void *pvParameters)
{
    while (true)
    {
        handle_energy_monitor();
        vTaskDelay(ENERGY_MONITOR_FREQUENCY / portTICK_PERIOD_MS);
    }
}

void ledcontrolTask(void *pvParameters)
{
    while (true)
    {
        handle_led_control();
        vTaskDelay(LED_CONTROL_FREQUENCY / portTICK_PERIOD_MS);
    }
}

void lightintensityTask(void *pvParameters)
{
    while (true)
    {
        handle_light_intensity();
        vTaskDelay(LIGHT_INTENSITY_FREQUENCY / portTICK_PERIOD_MS);
    }
}
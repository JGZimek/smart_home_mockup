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
#include "env_measurement.hpp"

#define ENV_MEASUREMENT_TAG "app_env_measurement"

bool init_env_measurement()
{
    ESP_LOGI(ENV_MEASUREMENT_TAG, "Initializing environmental measurement...");

    return true;
}

void handle_env_measurement()
{
    ESP_LOGI(ENV_MEASUREMENT_TAG, "Handling environmental measurement...");
}
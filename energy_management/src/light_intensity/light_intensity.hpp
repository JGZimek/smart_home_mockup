#pragma once

#include <Arduino.h>
#include "esp_log.h"

/*
 * @brief Initializes the light intensity sensor.
 */
bool init_light_intensity();

/*
 * @brief Handles the light intensity sensor.
 */
void handle_light_intensity();
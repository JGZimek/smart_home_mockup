#pragma once

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/**
 * @brief Sets up the ESP32 system, initializes components, and starts scheduling.
 *
 * This function initializes the hardware and software components
 * and sets up all necessary FreeRTOS tasks for the application.
 *
 * @return true if the setup and initialization were successful, false otherwise.
 */
bool esp_setup();
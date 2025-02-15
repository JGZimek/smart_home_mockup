#pragma once

#include <Arduino.h>
#include "esp_log.h"

/*
 * @brief Initializes the LED control.
 */
bool init_led_control();

/*
 * @brief Handles the LED control.
 */
void handle_led_control();
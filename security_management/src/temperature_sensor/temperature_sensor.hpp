#pragma once

#include <Arduino.h>

/**
 * @brief Initializes the temperature sensor.
 *
 * Configures the GPIO pins used for controlling the PIR sensors.
 * @return true if initialization was successful, false otherwise.
 */
bool init_temperature_sensor();

/**
 * @brief Handles the PIR sensors.
 *
 * This function should be called periodically to manage the PIR sensor inputs.
 */
void handle_temperature_sensor();

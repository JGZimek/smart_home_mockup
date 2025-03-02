#pragma once 

#include <Arduino.h>

/**
 * @brief Initializes the sensor.
 *
 * Configures the GPIO pin used for controlling the buzzer.
 * @return true if initialization was successful, false otherwise.
 */
bool init_fire_sensor();

/**
 * @brief Handles the sensor. - specyfy what does the system do when the sensor detects a fire
 *
 * This function should be called periodically to manage the buzzer's state.
 */
void handle_fire_sensor();
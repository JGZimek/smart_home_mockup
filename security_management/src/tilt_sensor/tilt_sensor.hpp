#pragma once 

#include <Arduino.h>

/**
 * @brief Initializes the sensor.
 * @return true if initialization was successful, false otherwise.
 */
bool init_tilt_sensor();

/**
 * @brief Handles the sensor. - specyfy what does the system do when the sensor detects a tilt
 * This function should be called periodically to manage the buzzer's state.
 */
void handle_tilt_sensor();
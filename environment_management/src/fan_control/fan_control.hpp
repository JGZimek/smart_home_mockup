#pragma once

#include <Arduino.h>
#include "esp_log.h"

// GPIO pin definitions for relay-controlled fans
#define RELAY_FAN_1_PIN 16  // Control pin for fan 1
#define RELAY_FAN_2_PIN 17  // Control pin for fan 2

/**
 * @brief Initializes fan control module.
 * 
 * Sets up the GPIO pins for the relay modules controlling the fans.
 * Initially turns off both fans.
 * 
 * @return true if initialization is successful, false otherwise.
 */
bool init_fan_control();

/**
 * @brief Controls the state of a specific fan.
 * 
 * @param state Desired state of the fan (true = ON, false = OFF).
 * @param pin GPIO pin connected to the relay controlling the fan.
 */
void control_fan(bool state, int pin);

/**
 * @brief Handles fan control logic.
 * 
 * Alternates fan states at a defined interval.
 */
void handle_fan_control();


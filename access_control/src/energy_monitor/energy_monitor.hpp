#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <DFRobot_INA219.h>
#include "esp_log.h"

/* Tag dla logów ESP */
#define ENERGY_MONITOR_TAG "energy_monitor"

/**
 * @brief Initializes the energy monitors.
 *
 * This function initializes two INA219 sensors for energy monitoring.
 *
 * @return true if both sensors initialized successfully, false otherwise.
 */
bool init_energy_monitor();

/**
 * @brief Reads energy production data.
 *
 * Reads power data from the INA219 sensor monitoring production.
 */
void read_energy_production();

/**
 * @brief Reads energy consumption data.
 *
 * Reads power data from the INA219 sensor monitoring consumption.
 */
void read_energy_consumption();

/**
 * @brief Handles energy monitoring logic.
 *
 * Reads power data from both INA219 sensors and logs results.
 */
void handle_energy_monitor();

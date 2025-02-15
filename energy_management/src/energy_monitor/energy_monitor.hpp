#pragma once

#include <Arduino.h>
#include "esp_log.h"

/*
 * @brief Initializes the energy monitor.
 */
bool init_energy_monitor();

/*
 * @brief Handles the energy monitor.
 */
void handle_energy_monitor();

float energy_consumption_monitor();

float energy_production_monitor();
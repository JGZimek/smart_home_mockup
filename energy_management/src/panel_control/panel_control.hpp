#pragma once

#include <Arduino.h>
#include "esp_log.h"

/*
 * @brief Initializes the panel control.
 */
bool init_panel_control();

/*
 * @brief Handles the panel control.
 */
void handle_panel_control();
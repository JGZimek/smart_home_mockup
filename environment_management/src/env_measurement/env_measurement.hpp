#pragma once

#include <Arduino.h>
#include "esp_log.h"
#include <Adafruit_BME280.h>

// Inicjalizacja czujnika BME280
bool init_env_measurement();

// Funkcja do obsługi pomiarów środowiskowych
void handle_env_measurement();

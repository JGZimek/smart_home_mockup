#pragma once

#include <Arduino.h>
#include <Adafruit_TSL2591.h>
#include <Wire.h>

// Inicjalizacja czujnika światła
bool init_light_intensity();

// Obsługa odczytów z czujnika światła
void handle_light_intensity();
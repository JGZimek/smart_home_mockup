#pragma once

#include <Arduino.h>
#include <Adafruit_TSL2591.h>
#include <Wire.h>
#include <SPI.h>
#include "esp_log.h"

// Obiekt czujnika światła
static void configureSensor();

// Inicjalizacja czujnika światła
bool init_light_intensity();

// Obsługa odczytów z czujnika światła
void handle_light_intensity();
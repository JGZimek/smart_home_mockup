#pragma once

#include <Arduino.h>
#include <Adafruit_TSL2591.h>

// Funkcja inicjalizująca czujnik
bool init_light_intensity();

// Funkcja do obsługi odczytów z czujnika
void handle_light_intensity();

// Funkcja do pobierania natężenia światła w luksach
float get_light_intensity();

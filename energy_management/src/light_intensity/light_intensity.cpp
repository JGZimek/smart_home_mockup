#include "light_intensity.hpp"
#include <Wire.h>

// Stwórz obiekt Adafruit_TSL2591
Adafruit_TSL2591 tsl2591;

// Zmienna do przechowywania ostatniego odczytanego poziomu światła
float light_intensity = 0.0;

// Funkcja do inicjalizacji czujnika TSL2591
bool init_light_intensity() {
  if (!tsl2591.begin()) {
    Serial.println(F("Nie udało się zainicjować czujnika TSL2591!"));
    return false;
  }
  
  // Ustawienia czujnika: czas integracji 100ms, niski zysk
  tsl2591.setTiming(TSL2591_INTEGRATIONTIME_100MS);
  tsl2591.setGain(TSL2591_GAIN_LOW);

  Serial.println(F("Czujnik TSL2591 zainicjowany pomyślnie"));
  return true;
}

// Funkcja do obsługi odczytu z czujnika
void handle_light_intensity() {
  uint16_t ch0 = tsl2591.getLuminosity(TSL2591_VISIBLE);  // Widoczna część widma
  uint16_t ch1 = tsl2591.getLuminosity(TSL2591_INFRARED); // Część podczerwona

  // Obliczanie natężenia światła w luksach
  light_intensity = tsl2591.calculateLux(ch0, ch1);

  // Wypisz wynik w serial monitorze
  Serial.print(F("Natężenie światła: "));
  Serial.print(light_intensity);
  Serial.println(F(" lux"));
}

// Funkcja do pobierania ostatniego odczytu natężenia światła
float get_light_intensity() {
  return light_intensity;
}

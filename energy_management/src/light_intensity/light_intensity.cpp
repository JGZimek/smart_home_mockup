#include "light_intensity.hpp"

// Tworzymy obiekt czujnika – jawnie przekazujemy identyfikator (opcjonalnie)
Adafruit_TSL2591 tsl2591 = Adafruit_TSL2591(2591);

// Zmienna do przechowywania ostatniego odczytu natężenia światła
static float light_intensity = 0.0;

bool init_light_intensity()
{
  // Inicjalizujemy magistralę I2C (upewnij się, że nie jest już wcześniej inicjalizowana w innym miejscu)
  Wire.begin();

  // Próba inicjalizacji czujnika
  if (!tsl2591.begin())
  {
    Serial.println(F("Nie udało się zainicjalizować czujnika TSL2591!"));
    return false;
  }

  // Ustawienia czujnika: czas integracji 100ms, niski zysk
  tsl2591.setTiming(TSL2591_INTEGRATIONTIME_100MS);
  tsl2591.setGain(TSL2591_GAIN_LOW);

  // Dajemy sensorowi chwilę na ustabilizowanie się (trochę dłużej niż czas integracji)
  delay(120);

  Serial.println(F("Czujnik TSL2591 zainicjalizowany pomyślnie"));
  return true;
}

void handle_light_intensity()
{
  // Odczyt wartości z czujnika
  uint16_t ch0 = tsl2591.getLuminosity(TSL2591_VISIBLE);  // Widoczna część widma
  uint16_t ch1 = tsl2591.getLuminosity(TSL2591_INFRARED); // Część podczerwona

  // Obliczanie natężenia światła w luksach
  light_intensity = tsl2591.calculateLux(ch0, ch1);

  // Wypisz wynik w Serial Monitorze
  Serial.print(F("Natężenie światła: "));
  Serial.print(light_intensity);
  Serial.println(F(" lux"));
}
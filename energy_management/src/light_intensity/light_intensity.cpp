#include "light_intensity.hpp"

#define LIGHT_INTENSITY_TAG "app_light_intensity"

Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);

static float light_intensity = 0.0;

static void configureSensor()
{
  // Ustawiamy zysk na 25x (Medium)
  tsl.setGain(TSL2591_GAIN_MED);
  // Ustawiamy czas integracji na 300ms
  tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);

  // Dla celów diagnostycznych wypisujemy konfigurację
  tsl2591Gain_t gain = tsl.getGain();
  switch (gain)
  {
  case TSL2591_GAIN_LOW:
    ESP_LOGI(LIGHT_INTENSITY_TAG, "Gain: 1x (Low)");
    break;
  case TSL2591_GAIN_MED:
    ESP_LOGI(LIGHT_INTENSITY_TAG, "Gain: 25x (Medium)");
    break;
  case TSL2591_GAIN_HIGH:
    ESP_LOGI(LIGHT_INTENSITY_TAG, "Gain: 428x (High)");
    break;
  case TSL2591_GAIN_MAX:
    ESP_LOGI(LIGHT_INTENSITY_TAG, "Gain: 9876x (Max)");
    break;
  default:
    ESP_LOGI(LIGHT_INTENSITY_TAG, "Gain: Nieznany");
    break;
  }

  ESP_LOGI(LIGHT_INTENSITY_TAG, "Timing: %d ms", ((tsl.getTiming() + 1) * 100));
}

bool init_light_intensity()
{
  Wire.begin();

  if (!tsl.begin())
  {
    ESP_LOGE(LIGHT_INTENSITY_TAG, "Nie udało się zainicjalizować czujnika TSL2591!");
    return false;
  }

  configureSensor();
  // Używamy opóźnienia, by sensor miał czas na stabilizację
  vTaskDelay(pdMS_TO_TICKS(350));

  ESP_LOGI(LIGHT_INTENSITY_TAG, "Czujnik TSL2591 zainicjalizowany pomyślnie");
  return true;
}

void handle_light_intensity()
{
  // Pobieramy 32-bitową wartość (górne 16 bitów to IR, dolne 16 bitów to pełny zakres)
  uint32_t lum = tsl.getFullLuminosity();
  uint16_t ir = lum >> 16;
  uint16_t full = lum & 0xFFFF;

  // Obliczamy natężenie światła (lux)
  light_intensity = tsl.calculateLux(full, ir);

  // Wypisujemy wynik przy użyciu ESP_LOG
  ESP_LOGI(LIGHT_INTENSITY_TAG, "Natężenie światła: %f lux", light_intensity);
}
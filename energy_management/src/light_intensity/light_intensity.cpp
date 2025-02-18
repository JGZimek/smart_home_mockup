#include "light_intensity.hpp"
#include "esp_log.h"
#include "freertos/semphr.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "LIGHT_INTENSITY";

// Obiekt czujnika z identyfikatorem diagnostycznym
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);

// Zmienna przechowująca ostatni odczyt natężenia światła
static float light_intensity = 0.0;

// Mutex chroniący operacje na magistrali I2C
static SemaphoreHandle_t i2c_mutex = NULL;

// Pomocnicza funkcja konfigurująca sensor (ustawienie zysku i czasu integracji)
static void configureSensor(void)
{
  // Uzyskaj dostęp do magistrali I2C
  xSemaphoreTake(i2c_mutex, portMAX_DELAY);

  // Ustawiamy zysk na 25x (Medium)
  tsl.setGain(TSL2591_GAIN_MED);
  // Ustawiamy czas integracji na 300ms
  tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);

  // Logowanie konfiguracji
  ESP_LOGI(TAG, "------------------------------------");

  tsl2591Gain_t gain = tsl.getGain();
  switch (gain)
  {
  case TSL2591_GAIN_LOW:
    ESP_LOGI(TAG, "Gain: 1x (Low)");
    break;
  case TSL2591_GAIN_MED:
    ESP_LOGI(TAG, "Gain: 25x (Medium)");
    break;
  case TSL2591_GAIN_HIGH:
    ESP_LOGI(TAG, "Gain: 428x (High)");
    break;
  case TSL2591_GAIN_MAX:
    ESP_LOGI(TAG, "Gain: 9876x (Max)");
    break;
  default:
    ESP_LOGI(TAG, "Gain: Nieznany");
    break;
  }

  ESP_LOGI(TAG, "Timing: %d ms", ((tsl.getTiming() + 1) * 100));
  ESP_LOGI(TAG, "------------------------------------");

  xSemaphoreGive(i2c_mutex);
}

bool init_light_intensity()
{
  // Inicjalizacja magistrali I2C
  Wire.begin();

  // Utwórz mutex, jeśli jeszcze nie został utworzony
  if (i2c_mutex == NULL)
  {
    i2c_mutex = xSemaphoreCreateMutex();
    if (i2c_mutex == NULL)
    {
      ESP_LOGE(TAG, "Nie udało się utworzyć mutexa dla I2C");
      return false;
    }
  }

  // Inicjalizacja czujnika – zabezpieczona mutexem
  xSemaphoreTake(i2c_mutex, portMAX_DELAY);
  bool sensorInitialized = tsl.begin();
  xSemaphoreGive(i2c_mutex);

  if (!sensorInitialized)
  {
    ESP_LOGE(TAG, "Nie udało się zainicjalizować czujnika TSL2591!");
    return false;
  }

  // Konfiguracja sensora
  configureSensor();

  // Odczekaj, aby sensor mógł się ustabilizować
  vTaskDelay(pdMS_TO_TICKS(350));

  ESP_LOGI(TAG, "Czujnik TSL2591 zainicjalizowany pomyślnie");
  return true;
}

void handle_light_intensity()
{
  uint32_t lum = 0;

  // Odczyt 32-bitowej wartości z sensora (zabezpieczony mutexem)
  xSemaphoreTake(i2c_mutex, portMAX_DELAY);
  lum = tsl.getFullLuminosity();
  xSemaphoreGive(i2c_mutex);

  uint16_t ir = lum >> 16;
  uint16_t full = lum & 0xFFFF;

  // Obliczanie natężenia światła (lux) – operacja również zabezpieczona mutexem
  xSemaphoreTake(i2c_mutex, portMAX_DELAY);
  light_intensity = tsl.calculateLux(full, ir);
  xSemaphoreGive(i2c_mutex);

  // Logowanie wyniku
  ESP_LOGI(TAG, "Natężenie światła: %f lux", light_intensity);
}
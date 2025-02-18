#include "light_intensity.hpp"
#include "esp_log.h"

#define LIGHT_INTENSITY_TAG "app_light_intensity"

// Globalny obiekt czujnika – identyfikator diagnostyczny 2591
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);

// Globalny mutex do ochrony dostępu do sensora (I2C)
SemaphoreHandle_t tslMutex = NULL;

// Globalna zmienna przechowująca ostatni odczyt natężenia światła (w luxach)
static float light_intensity = 0.0;

bool init_light_intensity()
{
  // Inicjalizacja magistrali I2C
  Wire.begin();

  // Utworzenie mutexu, jeśli jeszcze nie został utworzony
  if (tslMutex == NULL)
  {
    tslMutex = xSemaphoreCreateMutex();
    if (tslMutex == NULL)
    {
      ESP_LOGE(LIGHT_INTENSITY_TAG, "Nie udało się utworzyć mutexa dla TSL2591");
      return false;
    }
  }

  // Inicjalizacja czujnika – dostęp do sensora zabezpieczamy mutexem
  if (!tsl.begin())
  {
    ESP_LOGE(LIGHT_INTENSITY_TAG, "Nie udało się zainicjalizować czujnika TSL2591!");
    return false;
  }
  else
  {
    ESP_LOGI(LIGHT_INTENSITY_TAG, "TSL2591 zainicjalizowany pomyślnie.");
  }

  // Konfiguracja czujnika – ustawiamy zysk i czas integracji
  if (xSemaphoreTake(tslMutex, portMAX_DELAY) == pdTRUE)
  {
    tsl.setGain(TSL2591_GAIN_MED);                // 25x (Medium)
    tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS); // 300ms
    xSemaphoreGive(tslMutex);
    ESP_LOGI(LIGHT_INTENSITY_TAG, "Konfiguracja TSL2591: Gain=25x, Timing=300ms.");
  }
  else
  {
    ESP_LOGE(LIGHT_INTENSITY_TAG, "Nie udało się uzyskać mutexa przy konfiguracji TSL2591.");
  }

  // Odczekaj chwilę, aby sensor się ustabilizował
  vTaskDelay(pdMS_TO_TICKS(350));

  ESP_LOGI(LIGHT_INTENSITY_TAG, "Inicjalizacja TSL2591 zakończona pomyślnie.");
  return true;
}

void handle_light_intensity()
{
  uint16_t broadband = 0, infrared = 0, visible = 0;

  // Odczyt wartości z sensora zabezpieczony mutexem
  if (xSemaphoreTake(tslMutex, portMAX_DELAY) == pdTRUE)
  {
    broadband = tsl.getLuminosity(TSL2591_FULLSPECTRUM);
    infrared = tsl.getLuminosity(TSL2591_INFRARED);
    xSemaphoreGive(tslMutex);
  }
  else
  {
    ESP_LOGE(LIGHT_INTENSITY_TAG, "Nie udało się uzyskać mutexa przy odczycie TSL2591.");
    return;
  }

  // Oblicz wartość widzialną – jak w działającym przykładzie
  visible = broadband - infrared;

  // Opcjonalnie: oblicz wartość w luxach (można też wykorzystać 'visible')
  if (xSemaphoreTake(tslMutex, portMAX_DELAY) == pdTRUE)
  {
    light_intensity = tsl.calculateLux(broadband, infrared);
    xSemaphoreGive(tslMutex);
  }
  else
  {
    ESP_LOGE(LIGHT_INTENSITY_TAG, "Nie udało się uzyskać mutexa przy kalkulacji lux.");
  }

  ESP_LOGI(LIGHT_INTENSITY_TAG, "Natężenie światła: Widzialne=%d, Lux=%f", visible, light_intensity);
}

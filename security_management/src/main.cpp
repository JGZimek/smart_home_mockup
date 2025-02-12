#include <Arduino.h>
#include "scheduling/scheduling.hpp"

#define MAIN_TAG "app_main"

/**
 * @brief Main setup function called once at startup.
 *
 * This function initializes the hardware and software components
 * and sets up all necessary FreeRTOS tasks for the application.
 */
void setup()
{
  Serial.begin(115200);
  while (!Serial) 
    ;

  if (!security_setup())
  {
    ESP_LOGE(MAIN_TAG, "Security setup failed");
    return;
  }

  // init_scheduling();
}

/**
 * @brief Main loop function.
 *
 * This function is typically used for the main application logic.
 * However, in this FreeRTOS-based project, the primary workload is
 * handled by FreeRTOS tasks. A small delay is included to ensure
 * the watchdog timer is not triggered due to inactivity.
 */
void loop()
{
  vTaskDelay(10 / portTICK_PERIOD_MS); // 10 ms delay to prevent watchdog reset
}

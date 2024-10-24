#include "pir.hpp"
#include "esp_log.h"
#include "../mqtt/mqtt.hpp"
#include "../buzzer/buzzer.hpp"

#define PIR_TAG "app_pir"

// Ustawienia dla czujników PIR
const byte pir_pins[] = {32, 33, 34, 35};                       // GPIO piny czujników PIR
const int num_sensors = sizeof(pir_pins) / sizeof(pir_pins[0]); // Liczba czujników PIR

unsigned long last_detection_time[num_sensors]; // Czas ostatniej detekcji

const int PIR_DELAY_MS = 5000; // Opóźnienie między detekcjami (5 sekund)

bool init_pir()
{
    for (int i = 0; i < num_sensors; i++)
    {
        pinMode(pir_pins[i], INPUT);
        last_detection_time[i] = 0; // Resetowanie czasów detekcji
        ESP_LOGI(PIR_TAG, "Initialized PIR sensor on GPIO %d", pir_pins[i]);
    }
    return true;
}

void handle_pir()
{
    unsigned long current_time = millis();

    for (int i = 0; i < num_sensors; i++)
    {
        int state = digitalRead(pir_pins[i]);

        if (state == HIGH && (current_time - last_detection_time[i] >= PIR_DELAY_MS))
        {
            ESP_LOGI(PIR_TAG, "Motion detected on PIR sensor %d (GPIO %d)", i + 1, pir_pins[i]);

            if (alarm_armed)
            { // Sprawdzenie, czy alarm jest uzbrojony
                ESP_LOGI(PIR_TAG, "Alarm is armed. Sending motion detection event and activating buzzer.");
                publish_pir_event(i + 1); // Publikowanie zdarzenia przez MQTT
                set_buzzer_alarm(true);   // Aktywacja alarmu (buzzer)
            }
            else
            {
                ESP_LOGI(PIR_TAG, "Alarm is not armed. No action taken.");
            }

            last_detection_time[i] = current_time; // Aktualizacja czasu detekcji
        }
    }
}

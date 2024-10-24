#include "pir.hpp"
#include "esp_log.h"
#include "../mqtt/mqtt.hpp"

#define PIR_TAG "app_pir"

// Ustawienia dla czujników PIR
const byte pir_pins[] = {32, 33, 34, 35};                       // GPIO piny, do których są podłączone czujniki PIR
const int num_sensors = sizeof(pir_pins) / sizeof(pir_pins[0]); // Liczba czujników PIR

// Dynamiczne tablice do przechowywania stanu czujników
unsigned long last_detection_time[sizeof(pir_pins) / sizeof(pir_pins[0])]; // Przechowuje czas ostatniej detekcji

// Czas opóźnienia między kolejnymi detekcjami dla każdego czujnika (w milisekundach)
const int PIR_DELAY_MS = 5000; // Minimum 5 sekund między wykryciami ruchu dla każdego czujnika

bool init_pir()
{
    // Inicjalizacja pinów PIR i resetowanie czasów detekcji
    for (int i = 0; i < num_sensors; i++)
    {
        pinMode(pir_pins[i], INPUT);
        last_detection_time[i] = 0; // Inicjalizujemy czasy detekcji na 0
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

        // Sprawdź, czy ruch został wykryty i czy upłynął wystarczający czas od ostatniej detekcji
        if (state == HIGH && (current_time - last_detection_time[i] >= PIR_DELAY_MS))
        {
            ESP_LOGI(PIR_TAG, "Motion detected on PIR sensor %d (GPIO %d)", i, pir_pins[i]);

            // Wywołaj funkcję publikującą zdarzenie z PIR z modułu MQTT
            publish_pir_event(i);

            last_detection_time[i] = current_time; // Zaktualizuj czas ostatniej detekcji
        }
    }
}

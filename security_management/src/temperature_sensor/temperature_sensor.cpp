#include "temperature_sensor.hpp"
#include "esp_log.h"
#include "../mqtt/mqtt.hpp"
#include "../buzzer/buzzer.hpp"
#include <Adafruit_Sensor.h>
#include <DHT.h>

#define TEMPERATURE_SENSOR_TAG "app_temperature_sensor"
#define DHTPIN 5
#define DHTTYPE DHT11  // Jeśli używasz DHT22, zmień na DHT22

DHT dht(DHTPIN, DHTTYPE);

bool init_temperature_sensor() {
    ESP_LOGI(TEMPERATURE_SENSOR_TAG, "Initializing temperature sensors...");
    dht.begin();
    return true;
}

void handle_temperature_sensor() {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Error from senfor!");
    } else {
        Serial.printf("Temperature: %.1f °C, Humidity: %.1f%%\n", temperature, humidity);
    }

    delay(2000);  // Odczyt co 2 sekundy
}

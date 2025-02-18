#include "env_measurement.hpp"
#include <Adafruit_BME280.h>
#include <Wire.h>

#define ENV_MEASUREMENT_TAG "app_env_measurement"
#define BME280_I2C_ADDRESS  0x76  // Adres BME280 (może być 0x77)

Adafruit_BME280 bme;

bool init_env_measurement() {
    ESP_LOGI(ENV_MEASUREMENT_TAG, "Initializing BME280 sensor...");
    
    if (!bme.begin(BME280_I2C_ADDRESS)) {
        ESP_LOGE(ENV_MEASUREMENT_TAG, "BME280 not found! Check wiring.");
        return false;
    }
    
    ESP_LOGI(ENV_MEASUREMENT_TAG, "BME280 initialized successfully.");
    return true;
}

void handle_env_measurement() {
    float temperature = bme.readTemperature();
    float humidity = bme.readHumidity();
    float pressure = bme.readPressure() / 100.0F;

    ESP_LOGI(ENV_MEASUREMENT_TAG, "Temperature: %.2f °C, Humidity: %.2f %%, Pressure: %.2f hPa",
             temperature, humidity, pressure);
}

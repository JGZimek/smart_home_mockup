#include "energy_monitor.hpp"
#include "config.h"  // Definicje adresów INA219

// Inicjalizacja czujników INA219 z adresami jak w sensors.h
static DFRobot_INA219_IIC ina219Primary(&Wire, INA219_I2C_ADDRESS4);  // Czujnik produkcji
static DFRobot_INA219_IIC ina219Secondary(&Wire, INA219_I2C_ADDRESS2); // Czujnik konsumpcji

bool init_energy_monitor() {
    ESP_LOGI(ENERGY_MONITOR_TAG, "Initializing energy monitors...");

    // Inicjalizacja magistrali I2C na ESP32
    Wire.begin(21, 22);  // SDA = GPIO 21, SCL = GPIO 22

    if (!ina219Primary.begin()) {
        ESP_LOGE(ENERGY_MONITOR_TAG, "Primary INA219 sensor not found! Check connections.");
        return false;
    }

    if (!ina219Secondary.begin()) {
        ESP_LOGE(ENERGY_MONITOR_TAG, "Secondary INA219 sensor not found! Check connections.");
        return false;
    }

    ESP_LOGI(ENERGY_MONITOR_TAG, "Both energy monitors initialized successfully.");
    return true;
}

void read_energy_production() {
    float voltage = ina219Primary.getBusVoltage_V();
    float current = ina219Primary.getCurrent_mA();
    float power = ina219Primary.getPower_mW();

    ESP_LOGI(ENERGY_MONITOR_TAG, "[PRODUCTION] Voltage: %.2f V, Current: %.2f mA, Power: %.2f mW",
             voltage, current, power);
}

void read_energy_consumption() {
    float voltage = ina219Secondary.getBusVoltage_V();
    float current = ina219Secondary.getCurrent_mA();
    float power = ina219Secondary.getPower_mW();

    ESP_LOGI(ENERGY_MONITOR_TAG, "[CONSUMPTION] Voltage: %.2f V, Current: %.2f mA, Power: %.2f mW",
             voltage, current, power);
}

void handle_energy_monitor() {
    read_energy_production();
    read_energy_consumption();
}

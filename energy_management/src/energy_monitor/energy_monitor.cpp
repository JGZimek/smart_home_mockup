#include "energy_monitor.hpp"

// static DFRobot_INA219_IIC production_ina219(&Wire, 0x40);  // Czujnik produkcji (adres 0x40)
static DFRobot_INA219_IIC consumption_ina219(&Wire, 0x45); // Czujnik konsumpcji (adres 0x45)

bool init_energy_monitor() {
    ESP_LOGI(ENERGY_MONITOR_TAG, "Initializing energy monitors...");

    // Inicjalizacja magistrali I2C na ESP32
    Wire.begin(21, 22);  // SDA = GPIO 21, SCL = GPIO 22

    // if (!production_ina219.begin()) {
    //     ESP_LOGE(ENERGY_MONITOR_TAG, "Production INA219 sensor not found! Check connections.");
    //     return false;
    // }

    if (!consumption_ina219.begin()) {
        ESP_LOGE(ENERGY_MONITOR_TAG, "Consumption INA219 sensor not found! Check connections.");
        return false;
    }

    ESP_LOGI(ENERGY_MONITOR_TAG, "Both energy monitors initialized successfully.");
    return true;
}

// void read_energy_production() {
//     float busVoltage = production_ina219.getBusVoltage_V();
//     float current_mA = production_ina219.getCurrent_mA();
//     float shuntVoltage = production_ina219.getShuntVoltage_mV();
//     float power_mW = production_ina219.getPower_mW();

//     ESP_LOGI(ENERGY_MONITOR_TAG, "[PRODUCTION] Voltage: %.2f V, Current: %.2f mA, Shunt Voltage: %.2f mV, Power: %.2f mW",
//              busVoltage, current_mA, shuntVoltage, power_mW);
// }

void read_energy_consumption() {
    float busVoltage = consumption_ina219.getBusVoltage_V();
    float current_mA = consumption_ina219.getCurrent_mA();
    float shuntVoltage = consumption_ina219.getShuntVoltage_mV();
    float power_mW = consumption_ina219.getPower_mW();

    ESP_LOGI(ENERGY_MONITOR_TAG, "[CONSUMPTION] Voltage: %.2f V, Current: %.2f mA, Shunt Voltage: %.2f mV, Power: %.2f mW",
             busVoltage, current_mA, shuntVoltage, power_mW);
}

void handle_energy_monitor() {
    //read_energy_production();
    read_energy_consumption();
}

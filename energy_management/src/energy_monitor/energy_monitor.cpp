#include "energy_monitor.hpp"

float consumption;
float production;

bool init_energy_monitor()
{
    ESP_LOGI("energy_monitor", "Initializing energy monitor...");
    
    return true;
}

void handle_energy_monitor()
{
    ESP_LOGI("energy_monitor", "Handling energy monitor...");
    consumption=energy_consumption_monitor();
    production=energy_production_monitor();
    ESP_LOGI("energy_monitor", "Consumption: %.2f kWh, Production: %.2f kWh", consumption, production);
}

float energy_consumption_monitor()
{
    return 0;
}

float energy_production_monitor()
{
    return 0;
}
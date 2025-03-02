#include "fire_sensor.hpp"
#include "esp_log.h"
#include "./buzzer/buzzer.hpp" // Include the buzzer module

#define SENSOR_PIN 4 // GPIO pin where the fire sensor is connected
#define SENSOR_TAG "fire_sensor"
 
static bool sensor_active = false; // variable for sensor state (not sure if needed) 

bool init_fire_sensor()
{
    pinMode(SENSOR_PIN, INPUT);   // Configure the buzzer pin as an input
    ESP_LOGI(SENSOR_TAG, "Fire sensor initialized on GPIO %d", SENSOR_PIN);

    return true;
}
    
void handle_fire_sensor()
{
    if (digitalRead(SENSOR_PIN) == LOW) // Check if the sensor senses fire
    {
        set_buzzer_alarm(true); // activate the alarm
        ESP_LOGI(SENSOR_TAG, "Fire detected!"); // Print a message to the serial monitor (i want it to be printed only once)

    }
    else
    {
        set_buzzer_alarm(false); //deactivate the alarm

        ESP_LOGI(SENSOR_TAG, "Fire is not detected, yay!"); // Print a message to the serial monitor (i want it to be printed only once)
    }
}



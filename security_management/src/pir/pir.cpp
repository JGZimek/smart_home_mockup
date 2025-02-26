#include "pir.hpp"
#include "esp_log.h"
#include "../mqtt/mqtt.hpp"
#include "../buzzer/buzzer.hpp"

#define PIR_PIN 5 
#define PIR_TAG "app_pir"
#define DEBUG 4 

static bool PIR_active = false; // variable for sensor state (not sure if needed) 


bool init_pir()
{
    ESP_LOGI(PIR_TAG, "Initializing PIR sensors...");

    pinMode(PIR_PIN, INPUT);   
    pinMode(DEBUG, OUTPUT); 
    digitalWrite(DEBUG, LOW); 
    ESP_LOGI(PIR_TAG, "PIR sensor initialized on GPIO %d", PIR_PIN);
    return true;
}

void handle_pir()
{

    if (digitalRead(PIR_PIN) == HIGH) // When motion is detected, the sensor outputs a HIGH signal
    {
        digitalWrite(DEBUG, HIGH); // Turn on the debug pin
        ESP_LOGI(PIR_TAG, "move detected!"); // Print a message to the serial monitor 

    }
    else
    {
        digitalWrite(DEBUG, LOW); //deactivate the alarm
        ESP_LOGI(PIR_TAG, "move is not detected, chill!"); // Print a message to the serial monitor
    }
}
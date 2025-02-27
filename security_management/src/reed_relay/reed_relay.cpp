#include "reed_relay.hpp"
#include "esp_log.h"

#define SENSOR_PIN 5 // GPIO pin where the fire sensor is connected
#define SENSOR_TAG "reed_relay"
#define LED_PIN 4

bool init_reed_relay()
{
    pinMode(SENSOR_PIN, INPUT);   // Configure the buzzer pin as an input
    pinMode(LED_PIN, OUTPUT); // Configure the LED pin as an output
    digitalWrite(LED_PIN, LOW); // Turn off the LED initially
    ESP_LOGI(SENSOR_TAG, "Reed relay initialized on GPIO %d", SENSOR_PIN);

    return true;
}
    
void handle_reed_relay()
{
    if (digitalRead(SENSOR_PIN) == HIGH) // Check if the reed relay is closed
    {
        digitalWrite(LED_PIN, HIGH); // Turn on the LED if the door is open
    }
    else
    {
        digitalWrite(LED_PIN, LOW); // Turn off the LED
    }
}
#include "tilt_sensor.hpp"
#include "esp_log.h"
#include "./buzzer/buzzer.hpp" // Include the buzzer module for further use

#define LED_PIN 4            // Pin diody LED (może to być GPIO13, zależnie od płytki)
#define TILT_SENSOR_PIN 5        // Pin czujnika przechyłu (może być np. GPIO2)

bool init_tilt_sensor() {
  pinMode(LED_PIN, OUTPUT);       // Ustawienie pinu diody jako wyjście
  pinMode(TILT_SENSOR_PIN, INPUT);   // Ustawienie pinu czujnika przechyłu jako wejście
  return true;
}

void handle_tilt_sensor() {
    int titlState = digitalRead(TILT_SENSOR_PIN); 
    delay(50); // Debounce - short delay  
    int newState = digitalRead(TILT_SENSOR_PIN); 
  
    if (titlState == newState) { // only stabile states are considered
        if (titlState == HIGH) {
            digitalWrite(LED_PIN, HIGH);
        } else {
    digitalWrite(LED_PIN, LOW);
        }
    }
} 
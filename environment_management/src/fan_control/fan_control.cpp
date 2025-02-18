#include "fan_control.hpp"

#define FAN_CONTROL_TAG "app_fan_control"

// Fan state variables
bool state1;
bool state2;

/**
 * @brief Initializes the fan control module.
 * 
 * Configures the GPIO pins as outputs and ensures both fans are initially turned off.
 * 
 * @return true if initialization is successful, false otherwise.
 */
bool init_fan_control()
{
    ESP_LOGI(FAN_CONTROL_TAG, "Initializing fan control...");

    pinMode(RELAY_FAN_1_PIN, OUTPUT);  // Set fan 1 relay pin as output
    pinMode(RELAY_FAN_2_PIN, OUTPUT);  // Set fan 2 relay pin as output

    // Ensure both fans are off initially
    digitalWrite(RELAY_FAN_1_PIN, LOW);
    digitalWrite(RELAY_FAN_2_PIN, LOW);

    return true;
}

/**
 * @brief Controls the state of a specific fan.
 * 
 * Turns the fan on or off based on the given state.
 * 
 * @param state Desired fan state (true = ON, false = OFF).
 * @param pin GPIO pin connected to the relay controlling the fan.
 */
void control_fan(bool state, int pin)
{
    if (state) {
        digitalWrite(pin, HIGH);  // Turn fan ON
        ESP_LOGI(FAN_CONTROL_TAG, "Fan ON (Pin %d)", pin);
    } else {
        digitalWrite(pin, LOW);   // Turn fan OFF
        ESP_LOGI(FAN_CONTROL_TAG, "Fan OFF (Pin %d)", pin);
    }
}

/**
 * @brief Handles fan control logic.
 * 
 * Alternates fan states at a defined interval.
 */
void handle_fan_control()
{
    state1 = false;  // Turn off fan 1
    state2 = true;   // Turn on fan 2
    
    control_fan(state1, RELAY_FAN_1_PIN);
    control_fan(state2, RELAY_FAN_2_PIN);

    ESP_LOGI(FAN_CONTROL_TAG, "Handling fan control...");
}

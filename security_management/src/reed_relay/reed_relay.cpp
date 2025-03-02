#include "reed_relay.hpp"
#include "esp_log.h"

#define NUM_SENSORS 3 // GPIO pin where the reed relay is connected
#define SENSOR_TAG "reed_relay"

const int reedPins[NUM_SENSORS] = {26, 25, 33};
int lastStates[NUM_SENSORS] = {LOW, LOW, LOW};
char printBuffer[50]; // Buffer for printing messages

bool init_reed_relay()
{
    for (int i = 0; i < NUM_SENSORS; i++) // Configure the reed relay pin as an input
    {
        pinMode(reedPins[i], INPUT);
        ESP_LOGI(SENSOR_TAG, "Reed relay no. %d initialized on GPIO %d", i+1 , reedPins[i]);

    }

    // initial information about the state of reed relays 
    for (int i = 0; i < NUM_SENSORS; i++)
    {
        if (digitalRead(reedPins[i]) == HIGH)
        {
            sprintf(printBuffer, "Window no. %d is closed", i+1);
            Serial.println(printBuffer);
            lastStates[i] = HIGH;
        }
        else
        {
            sprintf(printBuffer, "Window no. %d is opened", i+1);
            Serial.println(printBuffer);
            lastStates[i] = LOW;
        }
    }
    return true;
}

void reed_relay_logic(int sensorIndex){
    if (digitalRead(reedPins[sensorIndex]) == HIGH) // Check if the reed relay is closed
    {
        if (lastStates[sensorIndex] == LOW){ // If it was open before
            sprintf(printBuffer, "Window no. %d is closed", sensorIndex+1);
            Serial.println(printBuffer);
            lastStates[sensorIndex] = HIGH;
        }
    }
    else
    {
        if (lastStates[sensorIndex] == HIGH){ // If it was closed before
            sprintf(printBuffer, "Window no. %d is open", sensorIndex+1);
            Serial.println(printBuffer);
            lastStates[sensorIndex] = LOW;
        }
    }
}
    
void handle_reed_relay()
{
    for (int i = 0; i < NUM_SENSORS; i++)
    {
        reed_relay_logic(i);
    }

}
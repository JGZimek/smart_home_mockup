#include "buzzer.hpp"
#include "esp_log.h"

#define BUZZER_PIN 15 // GPIO pin where the buzzer is connected
#define BUZZER_TAG "app_buzzer"

static bool alarm_active = false; // Zmienna, która przechowuje stan alarm

// Variables to manage beep sequences
static int beep_duration_ms = 500;
static int beep_count = 0;
static unsigned long last_beep_time = 0;
static bool is_beeping = false;

bool init_buzzer()
{
    pinMode(BUZZER_PIN, OUTPUT);   // Configure the buzzer pin as an output
    digitalWrite(BUZZER_PIN, LOW); // Ensure the buzzer is off initially
    ESP_LOGI(BUZZER_TAG, "Buzzer initialized on GPIO %d", BUZZER_PIN);
    return true;
}
    
void handle_buzzer()
{
    unsigned long current_time = millis();

    if (is_beeping && beep_count > 0)
    {
        if (current_time - last_beep_time >= beep_duration_ms)
        {
            digitalWrite(BUZZER_PIN, LOW); // Turn off the buzzer
            last_beep_time = current_time;
            beep_count--; // Decrease the number of remaining beeps
            delay(100);   // Small delay between beeps
            if (beep_count > 0)
            {
                digitalWrite(BUZZER_PIN, HIGH); // Turn on the buzzer for the next beep
            }
            else
            {
                is_beeping = false; // Stop beeping
            }
        }
    }

    if (alarm_active)
    {
        digitalWrite(BUZZER_PIN, HIGH); // If alarm is active, keep the buzzer on
    }
    else
    {
        digitalWrite(BUZZER_PIN, LOW); // Turn off the buzzer if alarm is not active
    }
}

void set_alarm_active(bool active){
    alarm_active = active;
}

void buzzer_short_beep(int duration_ms)
{
    digitalWrite(BUZZER_PIN, HIGH); // Turn the buzzer on
    delay(duration_ms);             // Wait for the specified duration
    digitalWrite(BUZZER_PIN, LOW);  // Turn the buzzer off
}

void buzzer_long_beep(int duration_ms)
{
    digitalWrite(BUZZER_PIN, HIGH); // Turn the buzzer on
    delay(duration_ms);             // Wait for the specified duration
    digitalWrite(BUZZER_PIN, LOW);  // Turn the buzzer off
}

void buzzer_beep_sequence(int duration_ms, int count)
{
    beep_duration_ms = duration_ms;
    beep_count = count;
    is_beeping = true;
    last_beep_time = millis();
    digitalWrite(BUZZER_PIN, HIGH); // Start the first beep
}

/*
    CURRENTLY ONLY THIS FUNCTION IS USED TO ACTIVATE THE ALARM
*/
void set_buzzer_alarm(bool alarm_on)
{
    alarm_active = alarm_on; // Set the alarm state based on MQTT message
    if (alarm_on)
    {
        digitalWrite(BUZZER_PIN, HIGH); // Turn on the buzzer
        ESP_LOGI(BUZZER_TAG, "Alarm activated");
    }
    else
    {
        digitalWrite(BUZZER_PIN, LOW); // Turn off the buzzer
        ESP_LOGI(BUZZER_TAG, "Alarm deactivated");
    }
}

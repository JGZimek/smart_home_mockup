#include "pir.hpp"
#include "esp_log.h"
#include "../mqtt/mqtt.hpp"
#include "../buzzer/buzzer.hpp"

#define PIR_TAG "app_pir"

// Constants for time display purposes
#define MINUTE 60000 
#define SECOND 1000 

#define MOTION_END_DELAY 4000  // debounce time for PIR sensor in milliseconds

char printBuffer[128];         // buffer for printing messages with variables

const int PIR_PINS[4] = {13, 12, 14, 27}; // PIR sensors pins
bool lastPirVal[4] = {LOW, LOW, LOW, LOW};  // Last state of PIR sensor
unsigned long endTime[4] = {0}; // time left to end the alarm
unsigned long timeAlarmON[4] = {0};  // Time of starting the alarm
unsigned long timeAlarmOFF[4] = {0};  // Time of ending the alarm

bool init_pir()
{
    ESP_LOGI(PIR_TAG, "Initializing PIR sensors...");
    for (int i = 0; i < 4; i++) {
        pinMode(PIR_PINS[i], INPUT);
    }
    vTaskDelay (1000 / portTICK_PERIOD_MS); // 1 sec delay for PIR sensor to initialize
    for (int i = 0; i < 4; i++) {
      ESP_LOGI(PIR_TAG, "PIR sensor no. %d initialized on GPIO:  %d", i+1,  PIR_PINS[i]);
    }
    return true;
}

// When potentiometer time - responsible is turned max-counterclockwise,
// the alarm stops working for a few seconds despite the presence of movement.
// Turning the potentiometer clockwise may be the solution,
// since it extends the time of high state of the PIR sensor. 
// But the issue is time precision, alarm may be turned on too long, it may be too hard to tune
// for our smart home simulation purposes.

// My solution is to make a debouncing mechanism for alarm disabling, becaude of its precision.

void pir_logic(int sensorIndex){
  int pirVal = digitalRead(PIR_PINS[sensorIndex]);

  if (pirVal == HIGH) {
      if (lastPirVal[sensorIndex] == LOW) {
          timeAlarmON[sensorIndex] = millis();  // Time of starting the alarm
          sprintf(printBuffer, "Motion detected by sensor %d!", sensorIndex + 1);
          set_buzzer_alarm(true); // Turn on the alarm
          Serial.println(printBuffer);
          lastPirVal[sensorIndex] = HIGH;
          endTime[sensorIndex] = 0; // Reset time to end the alarm
      }
  }

  if (pirVal == LOW) {
      if (lastPirVal[sensorIndex] == HIGH && endTime[sensorIndex] == 0) { // If the sensor isn't detecting anymore, start the timer
          endTime[sensorIndex] = millis();
      }

      if (endTime[sensorIndex] > 0 && millis() - endTime[sensorIndex] >= MOTION_END_DELAY) { // If the sensor isn't detecting anymore for a while, end the alarm
          timeAlarmOFF[sensorIndex] = millis();
          set_buzzer_alarm(false);  // Turn off the alarm
          sprintf(printBuffer, "Alarm from sensor %d stopped. It lasted %lu s", sensorIndex + 1, (timeAlarmOFF[sensorIndex] - timeAlarmON[sensorIndex]) / 1000);
          Serial.println(printBuffer);
          lastPirVal[sensorIndex] = LOW;
          endTime[sensorIndex] = 0; // reset debouncing timer
      }
  } else {
      endTime[sensorIndex] = 0; // If PIR detects motion again, reset timer
  }
}

void handle_pir()
{
  for (int i = 0; i < 4; i++) {
    pir_logic(i);
}
}

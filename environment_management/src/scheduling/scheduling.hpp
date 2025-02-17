#pragma once

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "../network/wifi/wifi.hpp"
#include "../network/mqtt/mqtt.hpp"
#include "../network/access_point/access_point.hpp"

/* Task priorities */
#define WIFI_TASK_PRIORITY 1
#define MQTT_TASK_PRIORITY 1

/* Core assignments */
#define WIFI_TASK_CORE 0
#define MQTT_TASK_CORE 1

/* Task stack size */
#define WIFI_TASK_STACK_SIZE 4096
#define MQTT_TASK_STACK_SIZE 4096

/* Event frequencies in ms */
#define WIFI_EVENT_FREQUENCY 1000
#define MQTT_EVENT_FREQUENCY 1000

/*
 * @brief Setup function for the ESP32.
 *
 * This function initializes the ESP32 hardware and software components.
 *
 * @return true if setup was successful, false otherwise
 */
bool esp_setup();

/*
 * @brief Initialize the scheduling tasks.
 *
 * This function creates the FreeRTOS tasks for the scheduling module.
 *
 * @return true if initialization was successful, false otherwise
 */
bool init_scheduling();

/*
 * @brief Task function for handling WiFi events.
 *
 * This function is responsible for handling WiFi events in the background.
 *
 * @param pvParameters pointer to task-specific data structure
 */
void wifiTask(void *pvParameters);

/*
 * @brief Task function for handling MQTT events.
 *
 * This function is responsible for handling MQTT events in the background.
 *
 * @param pvParameters pointer to task-specific data structure
 */
void mqttTask(void *pvParameters);
#pragma once

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "../network/wifi/wifi.hpp"
#include "../network/mqtt/mqtt.hpp"
#include "../network/button/button.hpp"
#include "../network/access_point/access_point.hpp"
#include "../fan_control/fan_control.hpp"
#include "../env_measurement/env_measurement.hpp"
#include "../led_control/led_control.hpp"
#include "../energy_monitor/energy_monitor.hpp"

/* Task priorities */
#define WIFI_TASK_PRIORITY 1
#define MQTT_TASK_PRIORITY 1
#define BUTTON_TASK_PRIORITY 2
#define FAN_CONTROL_TASK_PRIORITY 2
#define ENV_MEASUREMENT_TASK_PRIORITY 1
#define LED_CONTROL_TASK_PRIORITY 1
#define ENERGY_MONITOR_TASK_PRIORITY 1

/* Core assignments */
#define WIFI_TASK_CORE 0
#define MQTT_TASK_CORE 1
#define BUTTON_TASK_CORE 0
#define FAN_CONTROL_TASK_CORE 0
#define ENV_MEASUREMENT_TASK_CORE 1
#define LED_CONTROL_TASK_CORE 1
#define ENERGY_MONITOR_TASK_CORE 1

/* Task stack size */
#define WIFI_TASK_STACK_SIZE 4096
#define MQTT_TASK_STACK_SIZE 4096
#define BUTTON_TASK_STACK_SIZE 4096
#define FAN_CONTROL_TASK_STACK_SIZE 4096
#define ENV_MEASUREMENT_TASK_STACK_SIZE 4096
#define LED_CONTROL_TASK_STACK_SIZE 4096
#define ENERGY_MONITOR_TASK_STACK_SIZE 4096

/* Event frequencies in ms */
#define WIFI_EVENT_FREQUENCY 1000
#define MQTT_EVENT_FREQUENCY 1000
#define BUTTON_EVENT_FREQUENCY 100
#define FAN_CONTROL_EVENT_FREQUENCY 1000
#define ENV_MEASUREMENT_EVENT_FREQUENCY 1000
#define LED_CONTROL_EVENT_FREQUENCY 1000
#define ENERGY_MONITOR_EVENT_FREQUENCY 1000

/**
 * @brief Setup function for the ESP32.
 *
 * This function initializes the ESP32 hardware and software components.
 *
 * @return true if setup was successful, false otherwise
 */
bool esp_setup();

/**
 * @brief Initialize the scheduling tasks.
 *
 * This function creates the FreeRTOS tasks for the scheduling module.
 *
 * @return true if initialization was successful, false otherwise
 */
bool init_scheduling();

/**
 * @brief Task function for handling WiFi events.
 *
 * This function is responsible for handling WiFi events in the background.
 *
 * @param pvParameters pointer to task-specific data structure
 */
void wifiTask(void *pvParameters);

/**
 * @brief Task function for handling MQTT events.
 *
 * This function is responsible for handling MQTT events in the background.
 *
 * @param pvParameters pointer to task-specific data structure
 */
void mqttTask(void *pvParameters);

/**
 * @brief Task function for handling button events.
 *
 * This function is responsible for handling button events in the background.
 *
 * @param pvParameters pointer to task-specific data structure
 */
void buttonTask(void *pvParameters);

/**
 * @brief Task function for handling fan control events.
 *
 * This function is responsible for handling fan control events in the background.
 *
 * @param pvParameters pointer to task-specific data structure
 */
void fanControlTask(void *pvParameters);

/**
 * @brief Task function for handling environmental measurement events.
 *
 * This function is responsible for handling environmental measurement events in the background.
 *
 * @param pvParameters pointer to task-specific data structure
 */
void envMeasurementTask(void *pvParameters);

/**
 * @brief Task function for handling LED control events.
 *
 * This function is responsible for handling LED control events in the background.
 *
 * @param pvParameters pointer to task-specific data structure
 */
void ledControlTask(void *pvParameters);

/**
 * @brief Task function for handling energy monitor events.
 *
 * This function is responsible for handling energy monitor events in the background.
 *
 * @param pvParameters pointer to task-specific data structure
 */
void energyMonitorTask(void *pvParameters);
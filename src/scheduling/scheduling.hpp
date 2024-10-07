#pragma once

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "../network/wifi_manager.hpp"
#include "../communication/mqtt.hpp"
#include "../communication/mqtt_topics.hpp"

/* Task priorities */
#define NETWORK_TASK_PRIORITY 0
#define COMMUNICATION_TASK_PRIORITY 1

/* Core assignments */
#define NETWORK_CORE 0
#define COMMUNICATION_CORE 1

/* Task stack size */
#define NETWORK_TASK_STACK_SIZE 4096
#define COMMUNICATION_TASK_STACK_SIZE 4096

/* Event frequencies in ms */
#define NETWORK_RECONNECT_FREQ 1000
#define COMMUNICATION_EVENT_FREQ 1000

// MQTT client and topics
extern WiFiClient wifiClient;
extern PubSubClient mqttClient;
extern mqtt_topics topics;

/**
 * @brief Sets up the ESP32 system, initializes components, and starts scheduling.
 *
 * This function initializes the hardware and software components
 * for the application.
 */
bool esp_setup();

/**
 * @brief Initializes the scheduling system.
 *
 * This function sets up the FreeRTOS tasks for the different modules.
 *
 * @return true if initialization was successful, false otherwise.
 */
bool initScheduling();

/**
 * @brief Task to handle network module activities.
 *
 * This task manages WiFi connections and other network-related functionalities.
 *
 * @param pvParameters Pointer to parameters passed to the task.
 */
void networkTask(void *pvParameters);

/**
 * @brief Task to handle communication module activities.
 *
 * This task manages communication functionalities such as MQTT.
 *
 * @param pvParameters Pointer to parameters passed to the task.
 */
void communicationTask(void *pvParameters);

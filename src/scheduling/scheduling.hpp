#pragma once

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "../wifi/wifi.hpp"
#include "../rfid/rfid.hpp"

/* Task priorities */
#define WIFI_TASK_PRIORITY 0
#define RFID_TASK_PRIORITY 1

/* Core assignments */
#define WIFI_CORE 0
#define RFID_CORE 1

/* Task stack size */
#define WIFI_TASK_STACK_SIZE 4096
#define RFID_TASK_STACK_SIZE 4096

/* Event frequencies in ms */
#define WIFI_RECONNECT_FREQ 1000
#define RFID_READ_FREQ 500

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
bool init_scheduling();

/**
 * @brief Task to handle network module activities.
 *
 * This task manages WiFi connections and other network-related functionalities.
 *
 * @param pvParameters Pointer to parameters passed to the task.
 */
void wifiTask(void *pvParameters);

/**
 * @brief Task to handle RFID module activities.
 *
 * This task manages RFID card reading and processing.
 *
 * @param pvParameters Pointer to parameters passed to the task.
 */
void rfidTask(void *pvParameters);
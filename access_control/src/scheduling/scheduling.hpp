#pragma once

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "../wifi/wifi.hpp"
#include "../rfid/rfid.hpp"
#include "../pinpad/pinpad.hpp"
#include "../mqtt/mqtt.hpp"
#include "../button/button.hpp"
#include "../access_point/access_point.hpp"

/* Task priorities */
#define WIFI_TASK_PRIORITY 0
#define RFID_TASK_PRIORITY 1
#define PINPAD_TASK_PRIORITY 2
#define MQTT_TASK_PRIORITY 5
#define BUTTON_TASK_PRIORITY 3

/* Core assignments */
#define WIFI_CORE 0
#define RFID_CORE 1
#define PINPAD_CORE 0
#define MQTT_CORE 0
#define BUTTON_CORE 1

/* Task stack size */
#define WIFI_TASK_STACK_SIZE 4096
#define RFID_TASK_STACK_SIZE 4096
#define PINPAD_TASK_STACK_SIZE 4096
#define MQTT_TASK_STACK_SIZE 4096
#define BUTTON_TASK_STACK_SIZE 2048

/* Event frequencies in ms */
#define WIFI_RECONNECT_FREQ 1000
#define RFID_READ_FREQ 300
#define PINPAD_READ_FREQ 100
#define MQTT_READ_FREQ 100
#define BUTTON_READ_FREQ 10

/* Button configuration */
#define BUTTON_PIN 2 // Define the GPIO pin for the button

/**
 * @brief Sets up the ESP32 system, initializes components, and starts scheduling.
 *
 * This function initializes the hardware and software components
 * for the application. If no configuration is found, it starts in Access Point mode
 * to allow the user to input WiFi and MQTT settings. Once the settings are saved,
 * it connects to WiFi and MQTT and proceeds to initialize the remaining modules.
 *
 * @return true if the setup and initialization were successful, false otherwise.
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
 * @brief Task to handle WiFi module activities.
 *
 * This task manages WiFi connections, monitors the connection status,
 * and attempts reconnection if the connection is lost.
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

/**
 * @brief Task to handle pinpad module activities.
 *
 * This task manages pinpad keypresses and PIN entry.
 *
 * @param pvParameters Pointer to parameters passed to the task.
 */
void pinpadTask(void *pvParameters);

/**
 * @brief Task to handle MQTT module activities.
 *
 * This task manages MQTT communication with the backend, ensuring
 * the connection is maintained and handling incoming messages.
 *
 * @param pvParameters Pointer to parameters passed to the task.
 */
void mqttTask(void *pvParameters);

/**
 * @brief Task to handle button module activities.
 *
 * This task checks for button long press events. If a long press is detected,
 * it triggers Access Point mode to allow reconfiguration of WiFi and MQTT settings.
 *
 * @param pvParameters Pointer to parameters passed to the task.
 */
void buttonTask(void *pvParameters);

/**
 * @brief Initializes remaining modules after WiFi and MQTT are ready.
 *
 * This function initializes RFID, pinpad, and scheduling. It ensures that
 * these modules are only initialized after successful connections to WiFi
 * and MQTT.
 *
 * @return true if all modules are initialized successfully, false otherwise.
 */
bool initialize_remaining_modules();
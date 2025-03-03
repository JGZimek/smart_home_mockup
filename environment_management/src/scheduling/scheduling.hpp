#pragma once

#include "Arduino.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/**
 * @brief Initialize the hardware and software environment.
 *
 * This function initializes the hardware and software environment
 * for the application. This includes setting up the FreeRTOS tasks
 * and any other necessary components.
 *
 * @return true if the environment setup was successful, false otherwise.
 */
bool environment_setup();

/**
 * @brief Initialize the FreeRTOS scheduler.
 *
 * This function initializes the FreeRTOS scheduler and creates
 * the necessary tasks for the application.
 *
 * @return true if the scheduling setup was successful, false otherwise.
 */
bool init_scheduling();

/**
 * @brief Task to handle the WiFi connection.
 *
 * This task is responsible for connecting to the WiFi network
 * and maintaining the connection. It will periodically check
 * the connection status and attempt to reconnect if necessary.
 *
 * @param pvParameters optional parameters to pass to the task.
 */
void wifiTask(void *pvParameters);

/**
 * @brief Task to handle the MQTT connection.
 *
 * This task is responsible for connecting to the MQTT broker
 * and maintaining the connection. It will periodically check
 * the connection status and attempt to reconnect if necessary.
 *
 * @param pvParameters optional parameters to pass to the task.
 */
void mqttTask(void *pvParameters);
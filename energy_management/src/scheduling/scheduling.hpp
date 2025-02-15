#pragma once

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "energy_monitor/energy_monitor.hpp"
#include "led_control/led_control.hpp"

/* Task priorities */
#define ENERGY_MONITOR_PRIORITY 1
#define LED_CONTROL_PRIORITY 2

/* Core assignments */
#define ENERGY_MONITOR_CORE 0
#define LED_CONTROL_CORE 1

/* Task stack size */
#define ENERGY_MONITOR_STACK_SIZE 4096
#define LED_CONTROL_STACK_SIZE 4096

/* Event frequencies in ms */
#define ENERGY_MONITOR_FREQUENCY 1000
#define LED_CONTROL_FREQUENCY 100

/**
 * @brief Sets up the ESP32 system, initializes components, and starts scheduling.
 *
 * This function initializes the hardware and software components
 * and sets up all necessary FreeRTOS tasks for the application.
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
 * @brief Task that handles the energy monitor.
 *
 * This task is responsible for handling the energy monitor.
 *
 * @param pvParameters Task parameters.
 */
void energymonitorTask(void *pvParameters);

/**
 * @brief Task that handles the LED control.
 *
 * This task is responsible for handling the LED control.
 *
 * @param pvParameters Task parameters.
 */
void ledcontrolTask(void *pvParameters);
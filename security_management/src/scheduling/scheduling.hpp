#pragma once

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "../wifi/wifi.hpp"
#include "../mqtt/mqtt.hpp"
#include "../pir/pir.hpp"
#include "../buzzer/buzzer.hpp"
#include "../fire_sensor/fire_sensor.hpp"
#include "../smoke_detector/smoke_detector.hpp"
#include "../tilt_sensor/tilt_sensor.hpp"


/* Task priorities */
#define WIFI_TASK_PRIORITY 0
#define MQTT_TASK_PRIORITY 1
#define PIR_TASK_PRIORITY 2
#define BUZZER_TASK_PRIORITY 3
#define FIRE_SENSOR_TASK_PRIORITY 4
#define SMOKE_DETECTOR_TASK_PRIORITY 5
#define TILT_SENSOR_TASK_PRIORITY 6 // not sure if this is right priority

/* Core assignments */
#define WIFI_CORE 0
#define MQTT_CORE 0
#define PIR_CORE 1
#define BUZZER_CORE 0
#define FIRE_SENSOR_CORE 1
#define SMOKE_DETECTOR_CORE 1
#define TILT_SENSOR_CORE 1 // not sure if this is right

/* Task stack size */
#define WIFI_TASK_STACK_SIZE 4096
#define MQTT_TASK_STACK_SIZE 4096
#define PIR_TASK_STACK_SIZE 4096
#define BUZZER_TASK_STACK_SIZE 2048
#define FIRE_SENSOR_TASK_STACK_SIZE 2048
#define SMOKE_DETECTOR_TASK_STACK_SIZE 2048
#define TILT_SENSOR_TASK_STACK_SIZE 2048 

/* Event frequencies in ms */
#define WIFI_RECONNECT_FREQ 1000
#define MQTT_READ_FREQ 100
#define PIR_READ_FREQ 100
#define BUZZER_READ_FREQ 100
#define FIRE_SENSOR_READ_FREQ 100
#define SMOKE_DETECTOR_READ_FREQ 100
#define TILT_SENSOR_READ_FREQ 100

/**
 * @brief Sets up the security system, initializes components, and starts scheduling.
 *
 * This function initializes the hardware and software components
 * for the security system. If no configuration is found, it starts in Access Point mode
 * to allow the user to input WiFi and MQTT settings. Once the settings are saved,
 * it connects to WiFi and MQTT and proceeds to initialize the remaining modules.
 *
 * @return true if the setup and initialization were successful, false otherwise.
 */
bool security_setup();

/**
 * @brief Initializes the scheduling system.
 *
 * This function sets up the FreeRTOS tasks for the different modules.
 *
 * @return true if initialization was successful, false otherwise.
 */
bool init_scheduling();

/**
 * @brief Task that handles WiFi connection.
 *
 * This task is responsible for handling the WiFi connection.
 *
 * @param pvParameters Task parameters
 */
void wifiTask(void *pvParameters);

/**
 * @brief Task that handles MQTT connection.
 *
 * This task is responsible for handling the MQTT connection.
 *
 * @param pvParameters Task parameters
 */
void mqttTask(void *pvParameters);

/**
 * @brief Task that handles PIR sensor.
 *
 * This task is responsible for handling the PIR sensor.
 *
 * @param pvParameters Task parameters
 */
void pirTask(void *pvParameters);

/**
 * @brief Task that handles buzzer.
 *
 * This task is responsible for handling the buzzer.
 *
 * @param pvParameters Task parameters
 */
void buzzerTask(void *pvParameters);

/**
 * @brief Task that handles fire sensor.
 *
 * This task is responsible for handling the fire sensor.
 *
 * @param pvParameters Task parameters
 */
void fireSensorTask(void *pvParameters);

/**
 * @brief Task that handles smoke detector.
 *
 * This task is responsible for handling the smoke detector.
 *
 * @param pvParameters Task parameters
 */
void smokeDetectorTask(void *pvParameters);

/**
 * @brief Task that handles tilt snesor.
 *
 * This task is responsible for handling the tilt sensor.
 *
 * @param pvParameters Task parameters
 */
void tiltSensorTask(void *pvParameters);
#pragma once

#include <Arduino.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include "mqtt_topics.hpp"
#include "../buzzer/buzzer.hpp"

/**
 * @brief Global variable indicating if the alarm is armed or not.
 */
extern bool alarm_armed; // Global variable

/**
 * @brief Initializes the MQTT client and connects to the broker.
 *
 * @return true if initialization was successful, false otherwise.
 */
bool init_mqtt();

/**
 * @brief Handles MQTT communication.
 *
 * This function should be called periodically to maintain the connection
 * and handle incoming and outgoing messages.
 */
void handle_mqtt();

/**
 * @brief Publishes a PIR event to the backend.
 *
 * Sends the PIR motion detection event to the backend for further processing.
 *
 * @param sensor_index The index of the PIR sensor that detected motion.
 */
void publish_pir_event(int sensor_index);

/**
 * @brief Publishes an RFID event to the backend.
 *
 * Sends the RFID card UID to the backend for authentication or other processing.
 *
 * @param uid The UID of the RFID card.
 */
void publish_rfid_event(const String &uid);

/**
 * @brief Publishes a pinpad event to the backend.
 *
 * Sends the entered pin code to the backend for verification.
 *
 * @param pin_code The entered pin code.
 */
void publish_pinpad_event(const String &pin_code);

/**
 * @brief Subscribes to relevant topics and processes incoming MQTT messages.
 *
 * Handles commands from the backend, such as controlling the buzzer and monitoring the alarm status.
 *
 * @param topic The MQTT topic.
 * @param payload The message payload.
 * @param length The length of the payload.
 */
void mqtt_callback(char *topic, byte *payload, unsigned int length);

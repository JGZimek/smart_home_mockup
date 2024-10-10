#pragma once

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include "mqtt_topics.hpp"
#include "../utilities/configuration.hpp"

/**
 * @brief Initializes the MQTT client and sets up the connection to the broker.
 *
 * @param mqttClient Reference to the PubSubClient instance used for MQTT communication.
 * @param wifiClient Reference to the WiFiClient instance used for network communication.
 * @param topics Reference to the mqtt_topics structure that holds all MQTT topics.
 * @return true if initialization was successful, false otherwise.
 */
bool initMQTT(PubSubClient &mqttClient, WiFiClient &wifiClient, const mqtt_topics &topics);

/**
 * @brief Connects to the MQTT broker and subscribes to topics.
 *
 * @param mqttClient Reference to the PubSubClient instance used for MQTT communication.
 * @param topics Reference to the mqtt_topics structure that holds all MQTT topics.
 * @return true if connection was successful, false otherwise.
 */
bool connectMQTT(PubSubClient &mqttClient, const mqtt_topics &topics);

/**
 * @brief Reconnects to the MQTT broker if the connection is lost.
 *
 * @param mqttClient Reference to the PubSubClient instance used for MQTT communication.
 * @param topics Reference to the mqtt_topics structure that holds all MQTT topics.
 * @return true if reconnection was successful, false otherwise.
 */
bool reconnectMQTT(PubSubClient &mqttClient, const mqtt_topics &topics);

/**
 * @brief Periodically checks and handles the MQTT connection, including reconnections.
 *
 * @param mqttClient Reference to the PubSubClient instance used for MQTT communication.
 * @param topics Reference to the mqtt_topics structure that holds all MQTT topics.
 */
void handleMQTT(PubSubClient &mqttClient, const mqtt_topics &topics);

/**
 * @brief Callback function to handle incoming MQTT messages.
 *
 * @param topic The topic on which the message was received.
 * @param payload The message payload.
 * @param length Length of the message payload.
 */
void mqttCallback(char *topic, byte *payload, unsigned int length);

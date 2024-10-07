#pragma once

#include <Arduino.h>
#include <WiFi.h>

#include "../utilities/configuration.hpp"

/**
 * @brief Initializes the WiFi module and attempts to connect for the first time.
 *
 * This function initializes WiFi settings and establishes the initial connection.
 *
 * @return true if initialization and connection were successful, false otherwise.
 */
bool initWiFi();

/**
 * @brief Connects to the WiFi network for the first time.
 *
 * @return true if connection was successful, false otherwise.
 */
bool connectWiFi();

/**
 * @brief Reconnects to the WiFi network if the connection is lost.
 *
 * @return true if reconnection was successful, false otherwise.
 */
bool reconnectWiFi();

/**
 * @brief Periodically checks the WiFi connection and handles reconnections if needed.
 *
 * This function should be called periodically to maintain the WiFi connection.
 */
void handleWiFi();

/**
 * @brief Custom WiFi event handler.
 *
 * This function handles custom WiFi events and logs them to the console.
 *
 * @param event WiFi event to handle.
 */
void wifiEventHandler(WiFiEvent_t event);

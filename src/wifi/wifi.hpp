#pragma once

#include <Arduino.h>
#include <WiFi.h>

#define MAX_RETRY_COUNT 10
#define MY_SSID "-"
#define MY_PASSWORD "-"

static bool was_connected = false;


/**
 * @brief Initializes the WiFi module and attempts to connect for the first time.
 *
 * This function initializes WiFi settings and establishes the initial connection.
 *
 * @return true if initialization and connection were successful, false otherwise.
 */
bool init_wifi();

/**
 * @brief Connects to the WiFi network for the first time.
 *
 * @return true if connection was successful, false otherwise.
 */
bool connect_wifi();

/**
 * @brief Reconnects to the WiFi network if the connection is lost.
 *
 * @return true if reconnection was successful, false otherwise.
 */
bool reconnect_wifi();

/**
 * @brief Periodically checks the WiFi connection and handles reconnections if needed.
 *
 * This function should be called periodically to maintain the WiFi connection.
 */
void handle_wifi();

/**
 * @brief Custom WiFi event handler.
 *
 * This function handles custom WiFi events and logs them to the console.
 *
 * @param event WiFi event to handle.
 */
void wifi_event_handler(WiFiEvent_t event);

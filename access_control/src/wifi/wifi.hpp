#pragma once

#include <Arduino.h>
#include <WiFi.h>

/**
 * @brief Structure to hold WiFi credentials and configuration status.
 */
struct WifiCredentials
{
    String ssid;
    String password;
    bool isConfigured;
};

/**
 * @brief Reads WiFi credentials and configuration status from Preferences.
 *
 * @return WifiCredentials structure containing SSID, password, and configuration status.
 */
WifiCredentials read_wifi_credentials();

/**
 * @brief Checks if WiFi is configured in Preferences.
 *
 * Reads the stored SSID, password, and configuration flag from Preferences.
 *
 * @return true if WiFi is configured, false otherwise.
 */
bool is_wifi_configured();

/**
 * @brief Initializes the WiFi module and attempts to connect.
 *
 * This function sets up the WiFi mode and tries to connect using saved credentials.
 *
 * @return true if initialization and connection were successful, false otherwise.
 */
bool init_wifi();

/**
 * @brief Connects to the WiFi network using stored credentials.
 *
 * This function reads credentials from Preferences and tries to connect.
 *
 * @return true if connection was successful, false otherwise.
 */
bool connect_wifi();

/**
 * @brief Reconnects to WiFi if disconnected.
 *
 * This function checks the WiFi status and reconnects if needed.
 *
 * @return true if reconnection was successful, false otherwise.
 */
bool reconnect_wifi();

/**
 * @brief Periodically handles WiFi connectivity.
 *
 * Should be called in a loop to maintain WiFi connection.
 */
void handle_wifi();

/**
 * @brief Custom WiFi event handler.
 *
 * Handles events such as connection, disconnection, and IP assignment.
 *
 * @param event The WiFi event to handle.
 */
void wifi_event_handler(WiFiEvent_t event);

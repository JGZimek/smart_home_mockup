#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/**
 * @brief Class for storing WiFi/MQTT configuration using NVS.
 *
 * This class manages saving and loading configuration data (WiFi and MQTT)
 * in the ESP32 non-volatile storage (NVS).
 */
class WiFiCredentialsStore
{
public:
    /**
     * @brief Constructor.
     * @param ns The namespace used for storing preferences (default is "wifi").
     */
    explicit WiFiCredentialsStore(const char *ns = "wifi");

    /**
     * @brief Checks if the configuration has been set.
     * @return true if configuration exists, false otherwise.
     */
    bool isConfigured() const;

    /**
     * @brief Saves the WiFi and MQTT configuration.
     * @param ssid WiFi network name.
     * @param password WiFi network password.
     * @param mqttBroker MQTT broker address.
     * @param mqttPort MQTT broker port.
     * @return true if the configuration was saved successfully.
     */
    bool save(const String &ssid, const String &password,
              const String &mqttBroker, int mqttPort);

    /**
     * @brief Loads the saved configuration.
     * @param ssid (Output) WiFi network name.
     * @param password (Output) WiFi network password.
     * @param mqttBroker (Output) MQTT broker address.
     * @param mqttPort (Output) MQTT broker port.
     * @return true if the configuration is complete.
     */
    bool load(String &ssid, String &password, String &mqttBroker,
              int &mqttPort);

private:
    const char *_namespace; ///< Namespace used for storing preferences.
};

/**
 * @brief Class for managing the Access Point (AP) mode and HTTP configuration server.
 *
 * The AP mode remains active until a long-press on the button is detected
 * (handled externally).
 */
class AccessPoint
{
public:
    /**
     * @brief Constructor.
     * @param apSSID The SSID for the Access Point.
     * @param apPassword The password for the Access Point.
     * @param port The HTTP server port (default is 80).
     */
    AccessPoint(const char *apSSID, const char *apPassword, uint16_t port = 80);

    /**
     * @brief Starts the Access Point and the HTTP server.
     */
    void start();

    /**
     * @brief Stops the Access Point and the HTTP server.
     */
    void stop();

    /**
     * @brief Processes incoming HTTP client requests.
     */
    void handleRequests();

    /**
     * @brief Runs the AP mode until an exit is requested.
     */
    void run();

    /**
     * @brief Checks if the AP mode is active.
     * @return true if AP mode is active.
     */
    static bool isAPActive();

    /**
     * @brief Signals that the AP mode should exit.
     */
    static void requestAPExit();

    /**
     * @brief Starts the AP mode in a separate FreeRTOS task.
     * @param apSSID The SSID for the Access Point.
     * @param apPassword The password for the Access Point.
     */
    static void startAPTask(const char *apSSID, const char *apPassword);

private:
    /**
     * @brief Configures the HTTP server routes.
     */
    void setupRoutes();

    /**
     * @brief Handles the HTTP GET request at the root ("/").
     */
    void handleRoot();

    /**
     * @brief Handles the HTTP POST request to save configuration.
     */
    void handleSave();

    const char *_apSSID;                    ///< Access Point SSID.
    const char *_apPassword;                ///< Access Point password.
    WebServer _server;                      ///< HTTP server instance.
    WiFiCredentialsStore _credentialsStore; ///< Object for storing configuration.

    // Static members controlling AP mode.
    static volatile bool _apModeActive;    ///< Indicates if AP mode is active.
    static volatile bool _apExitRequested; ///< Indicates if exit from AP mode has been requested.
    static TaskHandle_t _apTaskHandle;     ///< Handle for the AP task.
};

#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/**
 * @brief Class for storing WiFi/MQTT configuration using NVS.
 */
class WiFiCredentialsStore
{
public:
    /**
     * @brief Constructs a new WiFiCredentialsStore object.
     * @param ns The namespace used for storing preferences. Defaults to "wifi".
     */
    explicit WiFiCredentialsStore(const char *ns = "wifi");

    /**
     * @brief Checks if configuration data is stored.
     * @return True if configuration exists, false otherwise.
     */
    bool isConfigured() const;

    /**
     * @brief Saves WiFi and MQTT configuration.
     * @param ssid WiFi network name.
     * @param password WiFi network password.
     * @param mqttBroker MQTT broker address.
     * @param mqttPort MQTT broker port.
     * @param mqttClientId MQTT client identifier.
     * @return True if the configuration was saved successfully.
     */
    bool save(const String &ssid, const String &password,
              const String &mqttBroker, int mqttPort, const String &mqttClientId);

    /**
     * @brief Loads stored configuration.
     * @param ssid (Output) WiFi network name.
     * @param password (Output) WiFi network password.
     * @param mqttBroker (Output) MQTT broker address.
     * @param mqttPort (Output) MQTT broker port.
     * @param mqttClientId (Output) MQTT client identifier.
     * @return True if configuration is complete.
     */
    bool load(String &ssid, String &password, String &mqttBroker,
              int &mqttPort, String &mqttClientId);

private:
    const char *_namespace; ///< Namespace used for storing preferences.
};

/**
 * @brief Class for managing the Access Point (AP) mode and HTTP configuration server.
 *
 * The AP mode remains active until the user triggers a long-press (handled externally).
 */
class AccessPoint
{
public:
    /**
     * @brief Constructs a new AccessPoint object.
     *
     * @param apSSID The SSID for the Access Point.
     * @param apPassword The password for the Access Point.
     * @param port The port for the HTTP server (default is 80).
     */
    AccessPoint(const char *apSSID, const char *apPassword, uint16_t port = 80);

    /**
     * @brief Starts the Access Point and HTTP server.
     */
    void start();

    /**
     * @brief Stops the Access Point and HTTP server.
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
     * @brief Checks whether AP mode is currently active.
     * @return True if AP mode is active.
     */
    static bool isAPActive();

    /**
     * @brief Signals that AP mode should exit.
     */
    static void requestAPExit();

    /**
     * @brief Launches the AP mode in a new FreeRTOS task.
     *
     * @param apSSID The SSID for the Access Point.
     * @param apPassword The password for the Access Point.
     */
    static void startAPTask(const char *apSSID, const char *apPassword);

private:
    /**
     * @brief Sets up HTTP server routes.
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
    WiFiCredentialsStore _credentialsStore; ///< Configuration storage.

    // Static members controlling AP mode.
    static volatile bool _apModeActive;    ///< Indicates whether AP mode is active.
    static volatile bool _apExitRequested; ///< Indicates whether exit from AP mode is requested.
    static TaskHandle_t _apTaskHandle;     ///< Handle for the AP task.
};

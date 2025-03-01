#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/**
 * @brief Class for managing persistent WiFi/MQTT configuration using NVS.
 */
class WiFiCredentialsStore
{
public:
    /**
     * @brief Constructor with namespace initialization.
     * @param ns The namespace to use for storing preferences (default is "wifi").
     */
    explicit WiFiCredentialsStore(const char *ns = "wifi");

    /**
     * @brief Checks if configuration data is stored.
     * @return True if configuration exists, false otherwise.
     */
    bool isConfigured() const;

    /**
     * @brief Saves WiFi and MQTT configuration.
     * @param ssid The WiFi network name.
     * @param password The WiFi password.
     * @param mqttBroker The MQTT broker address.
     * @param mqttPort The MQTT broker port.
     * @param mqttClientId The MQTT client identifier.
     * @return True if the save operation was successful.
     */
    bool save(const String &ssid, const String &password,
              const String &mqttBroker, int mqttPort, const String &mqttClientId);

    /**
     * @brief Loads stored configuration.
     * @param ssid (Output) The WiFi network name.
     * @param password (Output) The WiFi password.
     * @param mqttBroker (Output) The MQTT broker address.
     * @param mqttPort (Output) The MQTT broker port.
     * @param mqttClientId (Output) The MQTT client identifier.
     * @return True if configuration is complete.
     */
    bool load(String &ssid, String &password, String &mqttBroker,
              int &mqttPort, String &mqttClientId);

private:
    const char *_namespace;
};

/**
 * @brief Class for managing the Access Point (AP) mode and HTTP server for configuration.
 *
 * The AP mode remains active until the user triggers a long-press of the button,
 * which calls the static method requestAPExit().
 */
class AccessPoint
{
public:
    /**
     * @brief Constructor.
     * @param apSSID The SSID of the access point.
     * @param apPassword The password of the access point.
     * @param port The port number for the HTTP server (default is 80).
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
     * @brief Runs the AP mode. The AP remains active until requestAPExit() is called.
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
     * @param apSSID The SSID for the AP.
     * @param apPassword The password for the AP.
     */
    static void startAPTask(const char *apSSID, const char *apPassword);

private:
    void setupRoutes();
    void handleRoot();
    void handleSave();

    const char *_apSSID;
    const char *_apPassword;
    WebServer _server;
    WiFiCredentialsStore _credentialsStore;

    // Static flags controlling AP mode.
    static volatile bool _apModeActive;
    static volatile bool _apExitRequested;
    // Static handle for the AP task.
    static TaskHandle_t _apTaskHandle;
};

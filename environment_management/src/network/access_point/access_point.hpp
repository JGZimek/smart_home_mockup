#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>

/**
 * @brief Class managing persistent storage of WiFi configuration and MQTT settings.
 */
class WiFiCredentialsStore
{
public:
    /**
     * @brief Constructor – allows you to set the namespace for Preferences.
     * @param ns Namespace, defaults to "wifi".
     */
    WiFiCredentialsStore(const char *ns = "wifi") : _namespace(ns) {}

    /**
     * @brief Checks if the configuration has already been saved.
     * @return true if the configuration exists, false otherwise.
     */
    bool isConfigured() const
    {
        Preferences preferences;
        preferences.begin(_namespace, true);
        bool configured = preferences.getBool("configured", false);
        preferences.end();
        return configured;
    }

    /**
     * @brief Saves configuration data.
     * @param ssid WiFi network name.
     * @param password WiFi network password.
     * @param mqttBroker MQTT broker address.
     * @param mqttPort MQTT broker port.
     * @param mqttClientId MQTT client identifier.
     * @return true if save was successful, false otherwise.
     */
    bool save(const String &ssid, const String &password,
              const String &mqttBroker, int mqttPort, const String &mqttClientId)
    {
        Preferences preferences;
        if (!preferences.begin(_namespace, false))
        {
            return false;
        }
        bool result = true;
        result &= preferences.putString("ssid", ssid);
        result &= preferences.putString("password", password);
        result &= preferences.putString("mqtt_broker", mqttBroker);
        result &= preferences.putInt("mqtt_port", mqttPort);
        result &= preferences.putString("mqtt_client_id", mqttClientId);
        result &= preferences.putBool("configured", true);
        preferences.end();
        return result;
    }

    /**
     * @brief Reads saved configuration data.
     * @param ssid (output) read SSID.
     * @param password (output) read password.
     * @param mqttBroker (output) read MQTT broker address.
     * @param mqttPort (output) read MQTT broker port.
     * @param mqttClientId (output) read MQTT client identifier.
     * @return true if configuration is complete, false otherwise.
     */
    bool load(String &ssid, String &password, String &mqttBroker,
              int &mqttPort, String &mqttClientId)
    {
        Preferences preferences;
        if (!preferences.begin(_namespace, true))
        {
            return false;
        }
        ssid = preferences.getString("ssid", "");
        password = preferences.getString("password", "");
        mqttBroker = preferences.getString("mqtt_broker", "");
        mqttPort = preferences.getInt("mqtt_port", 1883);
        mqttClientId = preferences.getString("mqtt_client_id", "");
        bool configured = preferences.getBool("configured", false);
        preferences.end();
        return configured && !ssid.isEmpty() && !password.isEmpty() && !mqttBroker.isEmpty();
    }

private:
    const char *_namespace;
};

/**
 * @brief Class supporting Access Point mode and HTTP server, allowing device configuration.
 */
class AccessPoint
{
public:
    /**
     * @brief Constructor.
     * @param apSSID Access Point's SSID.
     * @param apPassword Access Point's password.
     * @param port Port on which the HTTP server should run (default 80).
     */
    AccessPoint(const char *apSSID, const char *apPassword, uint16_t port = 80);

    /**
     * @brief Starts the Access Point mode and HTTP server.
     */
    void start();

    /**
     * @brief Stops the Access Point mode and HTTP server.
     */
    void stop();

    /**
     * @brief Handles incoming HTTP requests.
     */
    void handleRequests();

    /**
     * @brief Runs the Access Point mode and HTTP server.
     */
    void run();

private:
    /**
     * @brief Sets up the HTTP routes.
     */
    void setupRoutes();

    /**
     * @brief Handles the root route ("/").
     */
    void handleRoot();

    /**
     * @brief Handles the WiFi configuration route ("/wifi").
     */
    void handleSave();

    const char *_apSSID;
    const char *_apPassword;
    WebServer _server;
    WiFiCredentialsStore _credentialsStore;
};
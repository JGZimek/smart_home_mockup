#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>

/**
 * @brief WiFiManager class.
 *
 * This class provides functionality for managing the WiFi connection.
 */
class WiFiManager
{
public:
    /**
     * @brief Default constructor for the WiFiManager class.
     *
     * This constructor initializes the WiFiManager instance.
     */
    WiFiManager();

    /**
     * @brief Default destructor for the WiFiManager class.
     *
     * This destructor cleans up the WiFiManager instance.
     */
    ~WiFiManager();

    /**
     * @brief Configure the WiFiManager with the given SSID and password.
     *
     * This function configures the WiFiManager with the given SSID and password.
     *
     * @param ssid the SSID of the WiFi network
     * @param password the password of the WiFi network
     * @return true if configuration was successful, false otherwise
     */
    bool configure(const String &ssid, const String &password);

    /**
     * @brief Check if the WiFiManager is configured.
     *
     * This function checks if the WiFiManager is configured.
     *
     * @return true if the WiFiManager is configured, false otherwise
     */
    bool isConfigured() const;

    /**
     * @brief Begin the WiFiManager.
     *
     * This function begins the WiFiManager.
     *
     * @return true if the WiFiManager was successfully started, false otherwise
     */
    bool begin();

    /**
     * @brief Connect to the WiFi network.
     *
     * This function connects to the WiFi network.
     *
     * @return true if the connection was successful, false otherwise
     */
    bool connect();

    /**
     * @brief Reconnect to the WiFi network.
     *
     * This function reconnects to the WiFi network.
     *
     * @return true if the reconnection was successful, false otherwise
     */
    bool reconnect();

    /**
     * @brief Handle WiFi events.
     *
     * This function handles WiFi events.
     */
    void handle();

private:
    /**
     * @brief WiFi credentials structure.
     *
     * This structure holds the WiFi credentials.
     */
    struct WifiCredentials
    {
        String ssid;
        String password;
        bool isConfigured;
    };

    /**
     * @brief Load WiFi credentials.
     *
     * This function loads the WiFi credentials.
     *
     * @return the WiFi credentials
     */
    WifiCredentials loadCredentials();

    /**
     * @brief Save WiFi credentials.
     *
     * This function saves the WiFi credentials.
     *
     * @param credentials the WiFi credentials to save
     * @return true if the credentials were saved successfully, false otherwise
     */
    bool saveCredentials(const WifiCredentials &credentials);

    bool wasConnected;

    static const int MAX_RETRY_COUNT = 10;
    static const int RECONNECT_DELAY_MS = 5000;
    static const char *WIFI_TAG;
    static WiFiManager *instance;

    /**
     * @brief WiFi event handler.
     *
     * This function handles WiFi events.
     *
     * @param event the WiFi event
     */
    static void wifiEventHandler(WiFiEvent_t event);
};

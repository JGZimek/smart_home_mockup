#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>

/**
 * @brief Class for managing the WiFi connection.
 *
 * This class encapsulates the functionality of configuring and maintaining
 * a WiFi connection. It loads/saves credentials using Preferences, attempts a
 * one-time connection in begin(), and then periodically checks and reconnects if needed.
 */
class WiFiManager
{
public:
    /**
     * @brief Constructs a new WiFiManager object.
     */
    WiFiManager();

    /**
     * @brief Destroys the WiFiManager object.
     */
    ~WiFiManager();

    /**
     * @brief Configures the WiFi credentials.
     *
     * @param ssid The SSID of the WiFi network.
     * @param password The password for the WiFi network.
     * @return true if configuration was successful, false otherwise.
     */
    bool configure(const String &ssid, const String &password);

    /**
     * @brief Checks if WiFi credentials have been configured.
     *
     * @return true if configured, false otherwise.
     */
    bool isConfigured() const;

    /**
     * @brief Begins the WiFi connection process.
     *
     * This method sets the WiFi mode to STA, clears previous connections,
     * registers the event handler, and performs a one-time connection attempt.
     * Execution continues regardless of the result.
     *
     * @return true if the one-time connection attempt succeeded, false otherwise.
     */
    bool begin();

    /**
     * @brief Performs a one-time connection attempt.
     *
     * @return true if connected, false otherwise.
     */
    bool connect();

    /**
     * @brief Attempts to reconnect if the connection is lost.
     *
     * @return true if reconnection was successful, false otherwise.
     */
    bool reconnect();

    /**
     * @brief Handles WiFi connection status.
     *
     * This method should be called periodically (e.g., in a FreeRTOS task) to
     * monitor the connection status and, if necessary, trigger reconnection attempts.
     */
    void handle();

private:
    /// Structure to store WiFi credentials.
    struct WifiCredentials
    {
        String ssid;
        String password;
        bool isConfigured;
    };

    /**
     * @brief Loads WiFi credentials from Preferences.
     *
     * @return A WifiCredentials structure containing the stored data.
     */
    WifiCredentials loadCredentials();

    /**
     * @brief Saves WiFi credentials to Preferences.
     *
     * @param credentials The credentials to save.
     * @return true if the save operation was successful, false otherwise.
     */
    bool saveCredentials(const WifiCredentials &credentials);

    bool _wasConnected; ///< Flag indicating if the connection was ever established.

    static const int MAX_RETRY_COUNT = 10;      ///< Maximum retry count (used in connect()).
    static const int RECONNECT_DELAY_MS = 5000; ///< Delay (ms) between reconnection attempts.
    static const char *WIFI_TAG;                ///< Logging tag for WiFiManager.
    static WiFiManager *instance;               ///< Static instance pointer for event handling.

    /**
     * @brief WiFi event handler function.
     *
     * This static function handles WiFi events and forwards them to the instance.
     *
     * @param event The WiFi event.
     */
    static void wifiEventHandler(WiFiEvent_t event);
};

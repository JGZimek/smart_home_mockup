#pragma once

#include <Arduino.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <Preferences.h>
#include <vector>
#include <functional>
#include "esp_log.h"

/**
 * @brief MqttManager class.
 *
 * This class provides functionality for managing the MQTT connection.
 * It loads the configuration from Preferences, connects to the broker,
 * handles reconnection and incoming messages, and offers an extensible interface
 * for publishing and registering topic-specific callbacks.
 */
class MqttManager
{
public:
    /**
     * @brief Constructor.
     */
    MqttManager();

    /**
     * @brief Destructor.
     */
    ~MqttManager();

    /**
     * @brief Configures the MQTT connection.
     *
     * Saves the MQTT broker address, port, and client ID to Preferences.
     *
     * @param broker MQTT broker address.
     * @param port MQTT broker port.
     * @param clientId MQTT client identifier.
     * @return true if the configuration was saved successfully, false otherwise.
     */
    bool configure(const String &broker, int port, const String &clientId);

    /**
     * @brief Checks if MQTT configuration is available.
     *
     * @return true if configuration exists and is valid, false otherwise.
     */
    bool isConfigured() const;

    /**
     * @brief Begins the MQTT connection.
     *
     * Loads the configuration from Preferences and attempts to connect to the MQTT broker.
     *
     * @return true if connected successfully, false otherwise.
     */
    bool begin();

    /**
     * @brief Handles MQTT events.
     *
     * Should be called periodically (e.g. in a FreeRTOS task) to maintain the connection.
     */
    void handle();

    /**
     * @brief Publishes a message to the specified topic.
     *
     * @param topic The MQTT topic.
     * @param message The message payload.
     * @return true if the publish was successful, false otherwise.
     */
    bool publishMessage(const String &topic, const String &message);

    /**
     * @brief Publishes an RFID event.
     *
     * @param uid The RFID card UID.
     * @return true if the publish was successful, false otherwise.
     */
    bool publishRfidEvent(const String &uid);

    /**
     * @brief Publishes a pinpad event.
     *
     * @param pinCode The entered pin code.
     * @return true if the publish was successful, false otherwise.
     */
    bool publishPinpadEvent(const String &pinCode);

    /**
     * @brief Registers a callback for a specific topic.
     *
     * When a message is received on the given topic, the callback is invoked.
     *
     * @param topic The MQTT topic.
     * @param callback The callback function.
     */
    void registerCallback(const String &topic, std::function<void(const String &topic, const String &payload)> callback);

    /**
     * @brief Subscribes to the specified topic.
     *
     * @param topic The MQTT topic to subscribe to.
     * @return true if the subscription is successful, false otherwise.
     */
    bool subscribeTopic(const String &topic);

    /**
     * @brief Checks if the MQTT client is connected.
     *
     * @return true if connected, false otherwise.
     */
    bool isConnected();

private:
    /**
     * @brief Structure for holding MQTT credentials.
     */
    struct MqttCredentials
    {
        String broker;
        int port;
        String clientId;
        bool isConfigured;
    };

    /**
     * @brief Loads MQTT credentials from Preferences.
     *
     * @return The loaded MQTT credentials.
     */
    MqttCredentials loadCredentials();

    /**
     * @brief Saves MQTT credentials to Preferences.
     *
     * @param credentials The credentials to save.
     * @return true if saved successfully, false otherwise.
     */
    bool saveCredentials(const MqttCredentials &credentials);

    /**
     * @brief Static callback for incoming MQTT messages.
     *
     * This function forwards the message to the registered callbacks.
     *
     * @param topic The topic on which the message was received.
     * @param payload The message payload.
     * @param length The length of the payload.
     */
    static void mqttCallback(char *topic, byte *payload, unsigned int length);

    WiFiClient wifiClient;       ///< WiFi client instance.
    PubSubClient mqttClient;     ///< PubSubClient instance.
    MqttCredentials credentials; ///< MQTT credentials.
    bool mqttConnected;          ///< Connection status flag.

    /**
     * @brief Structure for mapping a topic to a callback.
     */
    struct TopicCallback
    {
        String topic;
        std::function<void(const String &topic, const String &payload)> callback;
    };

    std::vector<TopicCallback> callbacks; ///< Registered callbacks.

    // Example topics (can be extended later)
    static constexpr const char *RFID_TOPIC = "smarthome/security/RFID/data";
    static constexpr const char *PINPAD_TOPIC = "smarthome/security/pinpad/data";

    static const int MAX_RETRY_COUNT = 10;
    static const int RECONNECT_DELAY_MS = 5000;

    static const char *MQTT_TAG;  ///< Logging tag.
    static MqttManager *instance; ///< Static pointer for the callback.
};

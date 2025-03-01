#include "mqtt.hpp"

#define MQTT_TAG_LOG "app_mqtt"

// Static member definitions
const char *MqttManager::MQTT_TAG = MQTT_TAG_LOG;
MqttManager *MqttManager::instance = nullptr;

MqttManager::MqttManager()
    : mqttClient(wifiClient),
      mqttConnected(false)
{
    instance = this;
}

MqttManager::~MqttManager()
{
    if (instance == this)
    {
        instance = nullptr;
    }
}

bool MqttManager::configure(const String &broker, int port, const String &clientId)
{
    MqttCredentials creds;
    creds.broker = broker;
    creds.port = port;
    creds.clientId = clientId;
    creds.isConfigured = (!broker.isEmpty() && port > 0 && !clientId.isEmpty());
    return saveCredentials(creds);
}

bool MqttManager::isConfigured() const
{
    MqttCredentials creds = const_cast<MqttManager *>(this)->loadCredentials();
    if (!creds.isConfigured || creds.broker.isEmpty() || creds.clientId.isEmpty())
    {
        ESP_LOGW(MQTT_TAG, "No MQTT configuration found.");
        return false;
    }
    ESP_LOGI(MQTT_TAG, "MQTT configured. Broker=%s, ClientId=%s", creds.broker.c_str(), creds.clientId.c_str());
    return true;
}

MqttManager::MqttCredentials MqttManager::loadCredentials()
{
    MqttCredentials creds = {"", 1883, "ESP32Client", false};
    Preferences preferences;
    // Using the same namespace as WiFi for simplicity.
    preferences.begin("wifi", true);
    creds.broker = preferences.getString("mqtt_broker", "");
    creds.port = preferences.getInt("mqtt_port", 1883);
    creds.clientId = preferences.getString("mqtt_client_id", "ESP32Client");
    creds.isConfigured = preferences.getBool("configured", false);
    preferences.end();
    return creds;
}

bool MqttManager::saveCredentials(const MqttCredentials &credentials)
{
    Preferences preferences;
    preferences.begin("wifi", false);
    bool result = true;
    result &= preferences.putString("mqtt_broker", credentials.broker);
    result &= preferences.putInt("mqtt_port", credentials.port);
    result &= preferences.putString("mqtt_client_id", credentials.clientId);
    result &= preferences.putBool("configured", credentials.isConfigured);
    preferences.end();
    return result;
}

bool MqttManager::begin()
{
    credentials = loadCredentials();
    if (credentials.broker.isEmpty())
    {
        ESP_LOGE(MQTT_TAG, "MQTT broker address is not configured.");
        return false;
    }

    mqttClient.setServer(credentials.broker.c_str(), credentials.port);
    mqttClient.setCallback(MqttManager::mqttCallback);

    ESP_LOGI(MQTT_TAG, "Connecting to MQTT broker %s:%d with client ID %s...",
             credentials.broker.c_str(), credentials.port, credentials.clientId.c_str());

    // Single connection attempt.
    bool connected = mqttClient.connect(credentials.clientId.c_str());
    if (connected)
    {
        ESP_LOGI(MQTT_TAG, "Connected to MQTT broker.");
        mqttConnected = true;
    }
    else
    {
        ESP_LOGW(MQTT_TAG, "Initial MQTT connection attempt failed.");
        mqttConnected = false;
    }
    return connected;
}

void MqttManager::handle()
{
    // If not connected, attempt reconnection every RECONNECT_DELAY_MS.
    if (!mqttClient.connected())
    {
        static unsigned long lastAttemptTime = 0;
        unsigned long currentTime = millis();
        if (currentTime - lastAttemptTime >= RECONNECT_DELAY_MS)
        {
            ESP_LOGW(MQTT_TAG, "MQTT connection lost. Attempting reconnection...");
            bool connected = mqttClient.connect(credentials.clientId.c_str());
            if (connected)
            {
                ESP_LOGI(MQTT_TAG, "Reconnected to MQTT broker.");
                mqttConnected = true;
            }
            else
            {
                ESP_LOGE(MQTT_TAG, "MQTT reconnection attempt failed.");
                mqttConnected = false;
            }
            lastAttemptTime = currentTime;
        }
    }
    mqttClient.loop();
}

bool MqttManager::publishMessage(const String &topic, const String &message)
{
    if (mqttClient.connected())
    {
        bool result = mqttClient.publish(topic.c_str(), message.c_str());
        ESP_LOGI(MQTT_TAG, "Published message to topic %s: %s", topic.c_str(), message.c_str());
        return result;
    }
    else
    {
        ESP_LOGW(MQTT_TAG, "MQTT is not connected. Cannot publish message.");
        return false;
    }
}

bool MqttManager::publishRfidEvent(const String &uid)
{
    String payload = "{\"value\": \"" + uid + "\"}";
    return publishMessage(RFID_TOPIC, payload);
}

bool MqttManager::publishPinpadEvent(const String &pinCode)
{
    String payload = "{\"value\": \"" + pinCode + "\"}";
    return publishMessage(PINPAD_TOPIC, payload);
}

void MqttManager::registerCallback(const String &topic, std::function<void(const String &topic, const String &payload)> callback)
{
    // Add new topic callback and subscribe to the topic.
    struct TopicCallback tc = {topic, callback};
    callbacks.push_back(tc);
    subscribeTopic(topic);
}

bool MqttManager::subscribeTopic(const String &topic)
{
    if (mqttClient.connected())
    {
        bool result = mqttClient.subscribe(topic.c_str());
        if (result)
        {
            ESP_LOGI(MQTT_TAG, "Subscribed to topic: %s", topic.c_str());
        }
        else
        {
            ESP_LOGE(MQTT_TAG, "Failed to subscribe to topic: %s", topic.c_str());
        }
        return result;
    }
    else
    {
        ESP_LOGW(MQTT_TAG, "Cannot subscribe, MQTT client not connected.");
        return false;
    }
}

bool MqttManager::isConnected()
{
    return mqttClient.connected();
}

void MqttManager::mqttCallback(char *topic, byte *payload, unsigned int length)
{
    String message;
    for (unsigned int i = 0; i < length; i++)
    {
        message += (char)payload[i];
    }
    ESP_LOGI(MQTT_TAG, "Received message on topic %s: %s", topic, message.c_str());

    if (instance != nullptr)
    {
        for (const auto &tc : instance->callbacks)
        {
            if (tc.topic.equals(topic))
            {
                tc.callback(String(topic), message);
            }
        }
    }
}

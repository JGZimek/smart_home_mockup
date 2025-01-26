#include "mqtt.hpp"
#include "esp_log.h"
#include <Preferences.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define MQTT_TAG "app_mqtt"

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// Definiowanie tematów MQTT
const char *mqtt_topics::rfid_topic = "smarthome/security/RFID/data";
const char *mqtt_topics::pinpad_topic = "smarthome/security/pinpad/data";

bool mqtt_connected = false;

MqttConfig read_mqtt_config()
{
    MqttConfig config = {"", 1883, "ESP32Client"}; // Domyślne wartości

    Preferences preferences;
    preferences.begin("wifi", true); // Tryb odczytu
    config.broker = preferences.getString("mqtt_broker", "");
    config.port = preferences.getInt("mqtt_port", 1883);
    config.clientId = preferences.getString("mqtt_client_id", "ESP32Client");
    preferences.end();

    return config;
}

bool init_mqtt()
{
    MqttConfig config = read_mqtt_config();

    if (config.broker.isEmpty())
    {
        ESP_LOGE(MQTT_TAG, "MQTT broker address is not configured.");
        return false;
    }

    mqttClient.setServer(config.broker.c_str(), config.port);
    mqttClient.setCallback(mqtt_callback);

    ESP_LOGI(MQTT_TAG, "Connecting to MQTT broker %s:%d with client ID %s...",
             config.broker.c_str(), config.port, config.clientId.c_str());

    if (mqttClient.connect(config.clientId.c_str()))
    {
        ESP_LOGI(MQTT_TAG, "Connected to MQTT broker.");
        mqtt_connected = true;
        return true;
    }
    else
    {
        ESP_LOGE(MQTT_TAG, "Failed to connect to MQTT broker.");
        return false;
    }
}

void handle_mqtt()
{
    if (!mqttClient.connected())
    {
        ESP_LOGW(MQTT_TAG, "MQTT connection lost. Reconnecting...");
        if (!init_mqtt())
        {
            ESP_LOGE(MQTT_TAG, "Reconnection to MQTT broker failed.");
        }
    }
    mqttClient.loop(); // Process MQTT messages
}

void publish_rfid_event(const String &uid)
{
    if (mqtt_connected)
    {
        String payload = "{\"value\": \"" + uid + "\"}";
        mqttClient.publish(mqtt_topics::rfid_topic, payload.c_str());
        ESP_LOGI(MQTT_TAG, "Published RFID event: %s", payload.c_str());
    }
    else
    {
        ESP_LOGW(MQTT_TAG, "MQTT is not connected. Cannot publish RFID event.");
    }
}

void publish_pinpad_event(const String &pin_code)
{
    if (mqtt_connected)
    {
        String payload = "{\"value\": \"" + pin_code + "\"}";
        mqttClient.publish(mqtt_topics::pinpad_topic, payload.c_str());
        ESP_LOGI(MQTT_TAG, "Published Pinpad event: %s", payload.c_str());
    }
    else
    {
        ESP_LOGW(MQTT_TAG, "MQTT is not connected. Cannot publish Pinpad event.");
    }
}

void mqtt_callback(char *topic, byte *payload, unsigned int length)
{
    String message;
    for (unsigned int i = 0; i < length; i++)
    {
        message += (char)payload[i];
    }
    ESP_LOGI(MQTT_TAG, "Received message on topic %s: %s", topic, message.c_str());
}

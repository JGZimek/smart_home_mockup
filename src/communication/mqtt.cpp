#include "mqtt.hpp"

#define MQTT_TAG "app_mqtt"

static int mqttReconnectAttempts = 0;

bool initMQTT(PubSubClient &mqttClient, WiFiClient &wifiClient, const mqtt_topics &topics)
{
    mqttClient.setClient(wifiClient);
    mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
    mqttClient.setCallback(mqttCallback);

    // Keep attempting to connect until successful
    while (!connectMQTT(mqttClient, topics))
    {
        ESP_LOGW(MQTT_TAG, "Initial MQTT connection failed. Retrying...");
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }

    return true;
}

bool connectMQTT(PubSubClient &mqttClient, const mqtt_topics &topics)
{
    ESP_LOGI(MQTT_TAG, "Attempting to connect to MQTT broker...");

    // Try connecting to the broker
    if (mqttClient.connect("ESP32Client"))
    {
        ESP_LOGI(MQTT_TAG, "Connected to MQTT broker.");

        // Subscribe to all topics dynamically from mqtt_topics
        for (const auto &topic : topics.get_all_topics())
        {
            mqttClient.subscribe(topic.c_str());
            ESP_LOGI(MQTT_TAG, "Subscribed to: %s", topic.c_str());
        }

        mqttReconnectAttempts = 0;
        return true;
    }
    else
    {
        ESP_LOGE(MQTT_TAG, "Failed to connect to MQTT broker.");
        return false;
    }
}

bool reconnectMQTT(PubSubClient &mqttClient, const mqtt_topics &topics)
{
    // If client is disconnected, attempt to reconnect
    if (!mqttClient.connected())
    {
        ESP_LOGW(MQTT_TAG, "Reconnecting to MQTT broker...");
        return connectMQTT(mqttClient, topics);
    }
    return true;
}

void handleMQTT(PubSubClient &mqttClient, const mqtt_topics &topics)
{
    // If disconnected, attempt reconnection
    if (!mqttClient.connected())
    {
        if (mqttReconnectAttempts < MAX_MQTT_RECONNECT_ATTEMPTS)
        {
            ESP_LOGI(MQTT_TAG, "Reconnection attempt %d/%d", mqttReconnectAttempts + 1, MAX_MQTT_RECONNECT_ATTEMPTS);
            reconnectMQTT(mqttClient, topics);
            mqttReconnectAttempts++;
        }
        else
        {
            ESP_LOGE(MQTT_TAG, "Exceeded max MQTT reconnection attempts.");
        }
    }
    else
    {
        mqttReconnectAttempts = 0;
        mqttClient.loop(); // Ensure client is looping to process incoming messages
    }
}

void mqttCallback(char *topic, byte *payload, unsigned int length)
{
    String message = String((char *)payload).substring(0, length);
    ESP_LOGI(MQTT_TAG, "Received message on topic %s: %s", topic, message.c_str());

    // Handle specific topics based on the received message, if necessary
}

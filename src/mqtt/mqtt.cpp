#include "mqtt.hpp"
#include "mqtt_topics.hpp"
#include "esp_log.h"
#include <WiFi.h>
#include <PubSubClient.h>

#define MQTT_TAG "app_mqtt"
#define MQTT_BROKER "your_mqtt_broker_address" // Zastąp adresem swojego brokera
#define MQTT_PORT 1883                         // Port MQTT

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// Flaga połączenia MQTT
bool mqtt_connected = false;

bool init_mqtt()
{
    mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
    mqttClient.setCallback(mqtt_callback);

    // Próba połączenia z brokerem
    if (mqttClient.connect("ESP32Client"))
    {
        ESP_LOGI(MQTT_TAG, "Connected to MQTT broker");
        mqttClient.subscribe(mqtt_topics::buzzer_control_topic); // Subskrypcja tematu do kontroli buzzera
        mqtt_connected = true;
        return true;
    }
    else
    {
        ESP_LOGE(MQTT_TAG, "Failed to connect to MQTT broker");
        return false;
    }
}

void handle_mqtt()
{
    if (!mqttClient.connected())
    {
        ESP_LOGW(MQTT_TAG, "MQTT connection lost. Reconnecting...");
        init_mqtt(); // Ponowna inicjalizacja połączenia
    }
    mqttClient.loop(); // Przetwarzanie wiadomości MQTT
}

void publish_pir_event(int sensor_index)
{
    const char *topic;
    if (sensor_index == 1)
    {
        topic = mqtt_topics::pir_topic_1;
    }
    else if (sensor_index == 2)
    {
        topic = mqtt_topics::pir_topic_2;
    }
    else
    {
        return;
    }

    if (mqtt_connected)
    {
        String payload = "{\"value\": 1}"; // Przykładowy payload
        mqttClient.publish(topic, payload.c_str());
        ESP_LOGI(MQTT_TAG, "Published PIR event on topic: %s", topic);
    }
    else
    {
        ESP_LOGW(MQTT_TAG, "MQTT is not connected. Cannot publish PIR event.");
    }
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

    if (String(topic) == mqtt_topics::buzzer_control_topic)
    {
        if (message == "{\"value\": 1}")
        {
            // Włącz buzzer
            buzzer_short_beep(200);
        }
        else if (message == "{\"value\": 0}")
        {
            // Wyłącz buzzer
            // (Opcjonalnie, wyłączanie buzzera)
        }
    }
}

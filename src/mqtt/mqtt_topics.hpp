#pragma once

/**
 * @brief Structure to manage MQTT topics for various modules.
 *
 * This structure holds all MQTT topics used by the system.
 */
struct mqtt_topics
{
    static const char *pir_topic_1;
    static const char *pir_topic_2;
    static const char *rfid_topic;
    static const char *pinpad_topic;
    static const char *buzzer_control_topic;
};

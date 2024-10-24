#include "mqtt_topics.hpp"

// Define the actual MQTT topics
const char *mqtt_topics::pir_topic_1 = "smarthome/security/PIR/1/status";
const char *mqtt_topics::pir_topic_2 = "smarthome/security/PIR/2/status";
const char *mqtt_topics::rfid_topic = "smarthome/security/RFID/data";
const char *mqtt_topics::pinpad_topic = "smarthome/security/pinpad/code";
const char *mqtt_topics::buzzer_control_topic = "smarthome/security/buzzer/status";

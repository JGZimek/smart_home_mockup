#pragma once

#include <string>
#include <vector>

/**
 * @brief Structure to manage MQTT topics for various devices in the security system.
 *
 * The structure is designed to generate topics in the format:
 *   smarthome/security/{device_type}/{device_name}/{status or data}
 * It keeps the system modular and easy to extend when adding new devices.
 */
class mqtt_topics
{
public:
    mqtt_topics(const std::string &home_id, const std::string &system_id)
        : root(home_id + "/" + system_id) {}

    // Generate topic paths for sensors
    std::string get_sensor_topic(const std::string &device_name) const
    {
        return root + "/sensors/" + device_name + "/status";
    }

    // Generate topic paths for actuators
    std::string get_actuator_topic(const std::string &device_name) const
    {
        return root + "/actuators/" + device_name + "/status";
    }

    // Generate topic paths for data devices (RFID, Pinpad, etc.)
    std::string get_data_topic(const std::string &device_name) const
    {
        return root + "/" + device_name + "/data";
    }

    // General topic for system-wide messages
    std::string get_general_topic() const
    {
        return root + "/general";
    }

    // Return a list of all available topics
    std::vector<std::string> get_all_topics() const
    {
        return {
            get_sensor_topic("tilt_sensor"),
            get_sensor_topic("PIR/1"),
            get_sensor_topic("PIR/2"),
            get_sensor_topic("radiation_sensor"),
            get_sensor_topic("flame_sensor"),
            get_actuator_topic("buzzer"),
            get_data_topic("RFID"),
            get_data_topic("pinpad"),
            get_general_topic()};
    }

private:
    std::string root; ///< Root of the topic tree, e.g., smarthome/security
};

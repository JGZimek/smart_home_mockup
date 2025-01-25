#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include "esp_log.h"

#define RST_PIN 22  // Configurable pin for RC522 reset
#define SS_PIN 5    // Configurable pin for RC522 SS (Slave Select)
#define LED_RFID 15 // GPIO pin where the LED is connected

/**
 * @brief Initializes the RFID reader (RC522 module).
 *
 * Initializes the SPI bus and the RC522 RFID reader.
 * Performs a self-test to ensure the reader is functioning correctly.
 * Uses ESP_LOG for logging the status.
 *
 * @return true if initialization was successful, false otherwise.
 */
bool init_RFID();

/**
 * @brief Periodically handles RFID card reading.
 *
 * This function reads the RFID card and processes it by logging the UID or sending it to the backend.
 */
void handle_RFID();

/**
 * @brief Reads an RFID card and retrieves its UID.
 *
 * This function checks if a new RFID card is present and reads its UID.
 * Errors or successes are logged using ESP_LOG.
 *
 * @return true if a card was successfully read, false otherwise.
 */
bool read_RFID();

/**
 * @brief Placeholder for publishing or storing the RFID card UID.
 *
 * This function is a placeholder for future integration with MQTT to send the RFID card UID to the backend.
 * Logs the UID using ESP_LOG.
 *
 * @param uid The UID of the RFID card.
 */
void publish_RFID_UID(const String &uid);

/**
 * @brief Logs the UID of the RFID card using ESP_LOG.
 *
 * This function prints the card's UID in a hexadecimal format for debugging or logging purposes.
 */
void print_RFID_UID();

/**
 * @brief Stops communication with the current RFID card.
 *
 * This function halts the communication with the RFID card and stops the encryption.
 * Logs the stop action using ESP_LOG.
 */
void stop_RFID_communication();

void light_up_LED();
#include "rfid.hpp"
#include "../mqtt/mqtt.hpp" // Dodaj ten include, aby używać publish_rfid_event
#include "esp_log.h"

#define RFID_TAG "app_rfid"

MFRC522 rfid(SS_PIN, RST_PIN);

bool init_RFID()
{
    SPI.begin();
    ESP_LOGI(RFID_TAG, "SPI initialized");

    rfid.PCD_Init();
    ESP_LOGI(RFID_TAG, "RFID reader PCD_Init called");

    pinMode(LED_RFID, OUTPUT);   // Initialize LED pin as output
    digitalWrite(LED_RFID, LOW); // Ensure LED is off initially

    ESP_LOGI(RFID_TAG, "RFID reader and LED initialized successfully.");
    return true;
}

void handle_RFID()
{
    if (read_RFID())
    {
        String uid = "";
        for (byte i = 0; i < rfid.uid.size; i++)
        {
            uid += String(rfid.uid.uidByte[i], HEX);
        }

        print_RFID_UID();          // Logowanie UID
        publish_RFID_UID(uid);     // Publikowanie UID na MQTT
        light_up_LED();            // Zapalanie diody
        stop_RFID_communication(); // Zakończenie komunikacji z kartą
    }
}

bool read_RFID()
{
    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    {
        ESP_LOGV(RFID_TAG, "No RFID card detected or failed to read.");
        return false;
    }

    ESP_LOGI(RFID_TAG, "RFID card detected.");
    return true;
}

void publish_RFID_UID(const String &uid)
{
    if (!uid.isEmpty()) // Upewnij się, że UID nie jest pusty
    {
        publish_rfid_event(uid); // Wywołanie funkcji publikującej z mqtt.cpp
        ESP_LOGI(RFID_TAG, "Published RFID UID to MQTT: %s", uid.c_str());
    }
    else
    {
        ESP_LOGW(RFID_TAG, "Cannot publish empty RFID UID.");
    }
}

void print_RFID_UID()
{
    String uid = "";
    for (byte i = 0; i < rfid.uid.size; i++)
    {
        uid += String(rfid.uid.uidByte[i], HEX);
    }
    ESP_LOGI(RFID_TAG, "RFID Card UID: %s", uid.c_str());
}

void stop_RFID_communication()
{
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
    ESP_LOGV(RFID_TAG, "Stopped communication with the RFID card.");
}

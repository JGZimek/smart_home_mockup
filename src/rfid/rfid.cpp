#include "rfid.hpp"
#include "esp_log.h"

#define RFID_TAG "app_rfid"

MFRC522 rfid(SS_PIN, RST_PIN);

bool init_RFID()
{
    SPI.begin();
    ESP_LOGI(RFID_TAG, "SPI initialized");

    rfid.PCD_Init();
    ESP_LOGI(RFID_TAG, "RFID reader PCD_Init called");

    ESP_LOGI(RFID_TAG, "RFID reader initialized successfully.");
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

        print_RFID_UID();
        publish_RFID_UID(uid);
        stop_RFID_communication();
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
    ESP_LOGI(RFID_TAG, "Placeholder: Publishing RFID UID.");
    // ESP_LOGI(RFID_TAG, "UID: %s", uid.c_str());
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

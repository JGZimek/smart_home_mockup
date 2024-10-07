#include "wifi_manager.hpp"

#define NETWORK_TAG "app_network"
#define MAX_RETRY_COUNT 10

static bool wasConnected = false;

bool initWiFi()
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    WiFi.onEvent(wifiEventHandler);

    while (!connectWiFi())
    {
        ESP_LOGW(NETWORK_TAG, "Initial connection failed. Retrying...");
        vTaskDelay(5000);
    }

    return true;
}

bool connectWiFi()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    int retryCount = 0;
    while (WiFi.status() != WL_CONNECTED && retryCount < MAX_RETRY_COUNT)
    {
        if (retryCount == 0)
        {
            ESP_LOGI(NETWORK_TAG, "Attempting to connect to WiFi...");
        }
        delay(1000);
        retryCount++;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        ESP_LOGI(NETWORK_TAG, "Connected to WiFi: %s", WiFi.SSID().c_str());
        wasConnected = true;
        return true;
    }
    else
    {
        ESP_LOGE(NETWORK_TAG, "Failed to connect to WiFi after %d attempts.", MAX_RETRY_COUNT);
        return false;
    }
}

bool reconnectWiFi()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        ESP_LOGW(NETWORK_TAG, "Reconnecting to WiFi...");
        return connectWiFi();
    }
    return true;
}

void handleWiFi()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        ESP_LOGW(NETWORK_TAG, "WiFi disconnected, attempting reconnection...");
        reconnectWiFi();
    }

    if (!wasConnected)
    {
        ESP_LOGI(NETWORK_TAG, "WiFi connection restored.");
        wasConnected = true;
    }
}

void wifiEventHandler(WiFiEvent_t event)
{
    switch (event)
    {
    case ARDUINO_EVENT_WIFI_STA_START:
        ESP_LOGI(NETWORK_TAG, "WiFi client started.");
        break;
    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
        ESP_LOGI(NETWORK_TAG, "WiFi client connected to SSID: %s", WiFi.SSID().c_str());
        wasConnected = true;
        break;
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
        ESP_LOGI(NETWORK_TAG, "WiFi client obtained IP address: %s", WiFi.localIP().toString().c_str());
        break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
        ESP_LOGE(NETWORK_TAG, "WiFi client disconnected.");
        wasConnected = false;
        reconnectWiFi();
        break;
    case ARDUINO_EVENT_WIFI_STA_LOST_IP:
        ESP_LOGE(NETWORK_TAG, "WiFi client lost IP address.");
        reconnectWiFi();
        break;
    default:
        ESP_LOGW(NETWORK_TAG, "Unhandled WiFi event.");
        break;
    }
}

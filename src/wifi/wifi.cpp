#include "wifi.hpp"

#define NETWORK_TAG "app_network"

bool init_wifi()
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    WiFi.onEvent(wifi_event_handler);

    while (!connect_wifi())
    {
        ESP_LOGW(NETWORK_TAG, "Initial connection failed. Retrying...");
        vTaskDelay(5000);
    }

    return true;
}

bool connect_wifi()
{
    WiFi.begin(MY_SSID, MY_PASSWORD);

    int retry_count = 0;
    while (WiFi.status() != WL_CONNECTED && retry_count < MAX_RETRY_COUNT)
    {
        if (retry_count == 0)
        {
            ESP_LOGI(NETWORK_TAG, "Attempting to connect to WiFi...");
        }
        delay(1000);
        retry_count++;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        ESP_LOGI(NETWORK_TAG, "Connected to WiFi: %s", WiFi.SSID().c_str());
        was_connected = true;
        return true;
    }
    else
    {
        ESP_LOGE(NETWORK_TAG, "Failed to connect to WiFi after %d attempts.", MAX_RETRY_COUNT);
        return false;
    }
}

bool reconnect_wifi()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        ESP_LOGW(NETWORK_TAG, "Reconnecting to WiFi...");
        return connect_wifi();
    }
    return true;
}

void handle_wifi()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        ESP_LOGW(NETWORK_TAG, "WiFi disconnected, attempting reconnection...");
        reconnect_wifi();
    }

    if (!was_connected)
    {
        ESP_LOGI(NETWORK_TAG, "WiFi connection restored.");
        was_connected = true;
    }
}

void wifi_event_handler(WiFiEvent_t event)
{
    switch (event)
    {
    case ARDUINO_EVENT_WIFI_STA_START:
        ESP_LOGI(NETWORK_TAG, "WiFi client started.");
        break;
    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
        ESP_LOGI(NETWORK_TAG, "WiFi client connected to SSID: %s", WiFi.SSID().c_str());
        was_connected = true;
        break;
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
        ESP_LOGI(NETWORK_TAG, "WiFi client obtained IP address: %s", WiFi.localIP().toString().c_str());
        break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
        ESP_LOGE(NETWORK_TAG, "WiFi client disconnected.");
        was_connected = false;
        reconnect_wifi();
        break;
    case ARDUINO_EVENT_WIFI_STA_LOST_IP:
        ESP_LOGE(NETWORK_TAG, "WiFi client lost IP address.");
        reconnect_wifi();
        break;
    default:
        ESP_LOGW(NETWORK_TAG, "Unhandled WiFi event.");
        break;
    }
}

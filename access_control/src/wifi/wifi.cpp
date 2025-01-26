#include "wifi.hpp"
#include <Preferences.h>

#define NETWORK_TAG "app_network"
#define MAX_RETRY_COUNT 10      // Maksymalna liczba prób połączenia
#define RECONNECT_DELAY_MS 5000 // Opóźnienie przed kolejną próbą połączenia

static bool was_connected = false; // Status połączenia WiFi

WifiCredentials read_wifi_credentials()
{
    WifiCredentials credentials = {"", "", false};

    Preferences preferences;
    preferences.begin("wifi", true); // Tryb odczytu
    credentials.ssid = preferences.getString("ssid", "");
    credentials.password = preferences.getString("password", "");
    credentials.isConfigured = preferences.getBool("configured", false);
    preferences.end();

    return credentials;
}

bool is_wifi_configured()
{
    WifiCredentials credentials = read_wifi_credentials();

    if (!credentials.isConfigured || credentials.ssid.isEmpty() || credentials.password.isEmpty())
    {
        ESP_LOGW(NETWORK_TAG, "WiFi not configured or credentials missing.");
        return false;
    }

    ESP_LOGI(NETWORK_TAG, "WiFi is configured. SSID=%s", credentials.ssid.c_str());
    return true;
}

bool init_wifi()
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect(); // Rozłączenie na początku, aby uniknąć konfliktów
    WiFi.onEvent(wifi_event_handler);

    return connect_wifi(); // Próba połączenia
}

bool connect_wifi()
{
    WifiCredentials credentials = read_wifi_credentials();

    if (credentials.ssid.isEmpty() || credentials.password.isEmpty())
    {
        ESP_LOGE(NETWORK_TAG, "WiFi credentials not found in Preferences.");
        return false;
    }

    ESP_LOGI(NETWORK_TAG, "Connecting to WiFi: SSID=%s", credentials.ssid.c_str());
    WiFi.begin(credentials.ssid.c_str(), credentials.password.c_str());

    int retry_count = 0;
    while (WiFi.status() != WL_CONNECTED && retry_count < MAX_RETRY_COUNT)
    {
        ESP_LOGI(NETWORK_TAG, "Attempting to connect... (%d/%d)", retry_count + 1, MAX_RETRY_COUNT);
        delay(1000); // Oczekiwanie na połączenie
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
        if (!reconnect_wifi())
        {
            ESP_LOGE(NETWORK_TAG, "WiFi reconnection failed.");
        }
    }
    else if (!was_connected)
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
        reconnect_wifi(); // Automatyczna próba połączenia
        break;
    default:
        ESP_LOGW(NETWORK_TAG, "Unhandled WiFi event.");
        break;
    }
}

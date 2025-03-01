#include "wifi.hpp"
#include "esp_log.h"
#include "../access_point/access_point.hpp"

// Define static members
const char *WiFiManager::WIFI_TAG = "app_wifi";
WiFiManager *WiFiManager::instance = nullptr;

WiFiManager::WiFiManager() : _wasConnected(false)
{
    instance = this;
}

WiFiManager::~WiFiManager()
{
    if (instance == this)
    {
        instance = nullptr;
    }
}

WiFiManager::WifiCredentials WiFiManager::loadCredentials()
{
    WifiCredentials credentials = {"", "", false};
    Preferences preferences;
    preferences.begin("wifi", true);
    credentials.ssid = preferences.getString("ssid", "");
    credentials.password = preferences.getString("password", "");
    credentials.isConfigured = preferences.getBool("configured", false);
    preferences.end();
    ESP_LOGI(WIFI_TAG, "Loaded credentials: SSID=%s, configured=%d", credentials.ssid.c_str(), credentials.isConfigured);
    return credentials;
}

bool WiFiManager::saveCredentials(const WifiCredentials &credentials)
{
    Preferences preferences;
    preferences.begin("wifi", false);
    bool result = true;
    result &= preferences.putString("ssid", credentials.ssid);
    result &= preferences.putString("password", credentials.password);
    result &= preferences.putBool("configured", credentials.isConfigured);
    preferences.end();
    return result;
}

bool WiFiManager::configure(const String &ssid, const String &password)
{
    WifiCredentials credentials;
    credentials.ssid = ssid;
    credentials.password = password;
    credentials.isConfigured = (!ssid.isEmpty() && !password.isEmpty());
    return saveCredentials(credentials);
}

bool WiFiManager::isConfigured() const
{
    WifiCredentials credentials = const_cast<WiFiManager *>(this)->loadCredentials();
    if (!credentials.isConfigured || credentials.ssid.isEmpty() || credentials.password.isEmpty())
    {
        ESP_LOGW(WIFI_TAG, "No WiFi credentials configured.");
        return false;
    }
    ESP_LOGI(WIFI_TAG, "WiFi credentials configured. SSID=%s", credentials.ssid.c_str());
    return true;
}

bool WiFiManager::begin()
{
    // Set WiFi mode to STA, disconnect any previous connection,
    // and register the event handler.
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    WiFi.onEvent(WiFiManager::wifiEventHandler);

    // Perform a one-time connection attempt.
    return connect();
}

bool WiFiManager::connect()
{
    WifiCredentials credentials = loadCredentials();
    if (credentials.ssid.isEmpty() || credentials.password.isEmpty())
    {
        ESP_LOGE(WIFI_TAG, "No WiFi credentials in Preferences.");
        return false;
    }

    ESP_LOGI(WIFI_TAG, "WiFi connecting: SSID=%s", credentials.ssid.c_str());
    WiFi.begin(credentials.ssid.c_str(), credentials.password.c_str());

    // Short delay to allow the connection procedure to start.
    vTaskDelay(100 / portTICK_PERIOD_MS);

    if (WiFi.status() == WL_CONNECTED)
    {
        ESP_LOGI(WIFI_TAG, "Connected to WiFi: %s", WiFi.SSID().c_str());
        _wasConnected = true;
        return true;
    }
    else
    {
        ESP_LOGW(WIFI_TAG, "Initial connection attempt failed.");
        return false;
    }
}

bool WiFiManager::reconnect()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        ESP_LOGW(WIFI_TAG, "Reconnecting to WiFi...");
        vTaskDelay(RECONNECT_DELAY_MS / portTICK_PERIOD_MS);
        return connect();
    }
    return true;
}

void WiFiManager::handle()
{
    // If AP mode is active, skip connection attempts.
    if (AccessPoint::isAPActive())
    {
        return;
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        // Attempt to reconnect periodically.
        static unsigned long lastAttemptTime = 0;
        unsigned long currentTime = millis();
        if (currentTime - lastAttemptTime >= RECONNECT_DELAY_MS)
        {
            ESP_LOGI(WIFI_TAG, "Re-attempting WiFi connection...");
            reconnect();
            lastAttemptTime = currentTime;
        }
    }
    else if (!_wasConnected)
    {
        ESP_LOGI(WIFI_TAG, "WiFi connection restored.");
        _wasConnected = true;
    }
}

void WiFiManager::wifiEventHandler(WiFiEvent_t event)
{
    if (instance == nullptr)
        return;

    switch (event)
    {
    case ARDUINO_EVENT_WIFI_STA_START:
        ESP_LOGI(instance->WIFI_TAG, "WiFi client started.");
        break;
    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
        ESP_LOGI(instance->WIFI_TAG, "Connected to SSID: %s", WiFi.SSID().c_str());
        instance->_wasConnected = true;
        break;
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
        ESP_LOGI(instance->WIFI_TAG, "IP address received: %s", WiFi.localIP().toString().c_str());
        break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
        ESP_LOGE(instance->WIFI_TAG, "WiFi disconnected.");
        instance->_wasConnected = false;
        break;
    default:
        ESP_LOGW(instance->WIFI_TAG, "Unhandled WiFi event.");
        break;
    }
}

#include "access_point.hpp"
#include "freertos/semphr.h"

// Global mutex to protect Preferences access
static SemaphoreHandle_t s_preferencesMutex = NULL;

WiFiCredentialsStore::WiFiCredentialsStore(const char *ns)
    : _namespace(ns)
{
    if (s_preferencesMutex == NULL)
    {
        s_preferencesMutex = xSemaphoreCreateMutex();
    }
}

bool WiFiCredentialsStore::isConfigured() const
{
    Preferences preferences;
    if (!preferences.begin(_namespace, true))
    {
        return false;
    }
    bool configured = preferences.getBool("configured", false);
    preferences.end();
    return configured;
}

bool WiFiCredentialsStore::save(const String &ssid, const String &password,
                                const String &mqttBroker, int mqttPort)
{
    // Protect Preferences access with a mutex
    if (s_preferencesMutex == NULL)
    {
        s_preferencesMutex = xSemaphoreCreateMutex();
    }
    xSemaphoreTake(s_preferencesMutex, portMAX_DELAY);

    Preferences preferences;
    if (!preferences.begin(_namespace, false))
    {
        xSemaphoreGive(s_preferencesMutex);
        return false;
    }

    bool result1 = preferences.putString("ssid", ssid);
    bool result2 = preferences.putString("password", password);
    bool result3 = preferences.putString("mqtt_broker", mqttBroker);
    bool result4 = preferences.putInt("mqtt_port", mqttPort);
    bool result5 = preferences.putBool("configured", true);
    preferences.end();

    xSemaphoreGive(s_preferencesMutex);

    Serial.printf("Preference writes: ssid:%d, password:%d, mqtt_broker:%d, mqtt_port:%d, configured:%d\n",
                  result1, result2, result3, result4, result5);

    return result1 && result2 && result3 && result4 && result5;
}

bool WiFiCredentialsStore::load(String &ssid, String &password, String &mqttBroker,
                                int &mqttPort)
{
    Preferences preferences;
    if (!preferences.begin(_namespace, true))
    {
        return false;
    }
    ssid = preferences.getString("ssid", "");
    password = preferences.getString("password", "");
    mqttBroker = preferences.getString("mqtt_broker", "");
    mqttPort = preferences.getInt("mqtt_port", 1883);
    bool configured = preferences.getBool("configured", false);
    preferences.end();
    return configured && !ssid.isEmpty() && !password.isEmpty() && !mqttBroker.isEmpty();
}

// Static member definitions
volatile bool AccessPoint::_apModeActive = false;
volatile bool AccessPoint::_apExitRequested = false;
TaskHandle_t AccessPoint::_apTaskHandle = NULL;

AccessPoint::AccessPoint(const char *apSSID, const char *apPassword, uint16_t port)
    : _apSSID(apSSID),
      _apPassword(apPassword),
      _server(port),
      _credentialsStore("wifi")
{
}

void AccessPoint::start()
{
    WiFi.softAP(_apSSID, _apPassword);
    IPAddress ip = WiFi.softAPIP();
    Serial.printf("Access Point started. IP address: %s\n", ip.toString().c_str());
    setupRoutes();
    _server.begin();
    Serial.println("HTTP server started.");
}

void AccessPoint::stop()
{
    _server.stop();
    WiFi.softAPdisconnect(true);
    Serial.println("Access Point stopped.");
}

void AccessPoint::handleRequests()
{
    _server.handleClient();
}

void AccessPoint::setupRoutes()
{
    _server.on("/", HTTP_GET, [this]()
               { handleRoot(); });
    _server.on("/save", HTTP_POST, [this]()
               { handleSave(); });
}

void AccessPoint::handleRoot()
{
    String html =
        "<html><body><h1>ESP Access Point</h1>"
        "<form method='POST' action='/save'>"
        "<label for='ssid'>WiFi SSID:</label>"
        "<input type='text' id='ssid' name='ssid' required><br>"
        "<label for='password'>WiFi Password:</label>"
        "<input type='password' id='password' name='password' required><br>"
        "<label for='mqtt_broker'>MQTT Broker Address:</label>"
        "<input type='text' id='mqtt_broker' name='mqtt_broker' required><br>"
        "<label for='mqtt_port'>MQTT Broker Port:</label>"
        "<input type='number' id='mqtt_port' name='mqtt_port' value='1883' required><br>"
        "<button type='submit'>Save</button>"
        "</form></body></html>";
    _server.send(200, "text/html", html);
}

void AccessPoint::handleSave()
{
    if (_server.hasArg("ssid") && _server.hasArg("password") &&
        _server.hasArg("mqtt_broker") && _server.hasArg("mqtt_port"))
    {
        String ssid = _server.arg("ssid");
        String password = _server.arg("password");
        String mqttBroker = _server.arg("mqtt_broker");
        int mqttPort = _server.arg("mqtt_port").toInt();

        if (_credentialsStore.save(ssid, password, mqttBroker, mqttPort))
        {
            Serial.printf("Saved settings: SSID=%s, PASSWORD=%s, MQTT Broker=%s, Port=%d\n",
                          ssid.c_str(), password.c_str(), mqttBroker.c_str(), mqttPort);
            _server.send(200, "text/html",
                         "<html><body><h1>Settings Saved. AP remains active until button long-press.</h1></body></html>");
        }
        else
        {
            _server.send(500, "text/html",
                         "<html><body><h1>Failed to save settings.</h1></body></html>");
        }
    }
    else
    {
        _server.send(400, "text/html",
                     "<html><body><h1>Missing parameters</h1></body></html>");
    }
}

void AccessPoint::run()
{
    _apModeActive = true;
    _apExitRequested = false;
    start();
    Serial.println("AP mode active. Awaiting user exit (long-press button).");
    while (!_apExitRequested)
    {
        handleRequests();
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    stop();
    _apModeActive = false;
}

bool AccessPoint::isAPActive()
{
    return _apModeActive;
}

void AccessPoint::requestAPExit()
{
    _apExitRequested = true;
}

void AccessPoint::startAPTask(const char *apSSID, const char *apPassword)
{
    if (_apTaskHandle == NULL)
    {
        AccessPoint *apInstance = new AccessPoint(apSSID, apPassword);
        BaseType_t result = xTaskCreate(
            [](void *param)
            {
                AccessPoint *ap = static_cast<AccessPoint *>(param);
                ap->run();
                delete ap;
                _apTaskHandle = NULL;
                vTaskDelete(NULL);
            },
            "APTask",
            4096,
            apInstance,
            1,
            &_apTaskHandle);
        if (result != pdPASS)
        {
            Serial.println("Failed to create AP task.");
            delete apInstance;
        }
    }
}

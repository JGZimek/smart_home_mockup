#include "access_point.hpp"

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
    IPAddress IP = WiFi.softAPIP();
    Serial.printf("Access Point started. IP address: %s\n", IP.toString().c_str());
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
               { this->handleRoot(); });
    _server.on("/save", HTTP_POST, [this]()
               { this->handleSave(); });
}

void AccessPoint::handleRoot()
{
    String html = "<html><body><h1>ESP Access Point</h1>"
                  "<form method='POST' action='/save'>"
                  "<label for='ssid'>WiFi SSID:</label>"
                  "<input type='text' id='ssid' name='ssid' required><br>"
                  "<label for='password'>WiFi Password:</label>"
                  "<input type='password' id='password' name='password' required><br>"
                  "<label for='mqtt_broker'>MQTT Broker Address:</label>"
                  "<input type='text' id='mqtt_broker' name='mqtt_broker' required><br>"
                  "<label for='mqtt_port'>MQTT Broker Port:</label>"
                  "<input type='number' id='mqtt_port' name='mqtt_port' value='1883' required><br>"
                  "<label for='mqtt_client_id'>MQTT Client ID (optional):</label>"
                  "<input type='text' id='mqtt_client_id' name='mqtt_client_id'><br>"
                  "<button type='submit'>Save</button></form>"
                  "</body></html>";
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
        String mqttClientId = _server.hasArg("mqtt_client_id") ? _server.arg("mqtt_client_id") : "ESP32Client";

        if (_credentialsStore.save(ssid, password, mqttBroker, mqttPort, mqttClientId))
        {
            Serial.printf("Saved settings: SSID=%s, PASSWORD=%s, MQTT Broker=%s, Port=%d, Client ID=%s\n",
                          ssid.c_str(), password.c_str(), mqttBroker.c_str(), mqttPort, mqttClientId.c_str());
            _server.send(200, "text/html",
                         "<html><body><h1>Settings Saved. AP will stop now.</h1></body></html>");
            stop();
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
    start();
    bool credentialsSaved = false;
    while (!credentialsSaved)
    {
        handleRequests();

        String ssid, password, mqttBroker, mqttClientId;
        int mqttPort = 0;
        if (_credentialsStore.load(ssid, password, mqttBroker, mqttPort, mqttClientId))
        {
            Serial.printf("[AccessPoint] Credentials saved. SSID=%s, MQTT Broker=%s\n", ssid.c_str(), mqttBroker.c_str());
            credentialsSaved = true;
        }
        else
        {
            Serial.println("[AccessPoint] Waiting for user to input new credentials...");
        }
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    stop();
}

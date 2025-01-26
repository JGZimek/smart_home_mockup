#include "access_point.hpp"

AccessPoint::AccessPoint(const char *ssid, const char *password)
    : ssid(ssid), password(password), server(80) {}

void AccessPoint::start()
{
    WiFi.softAP(ssid, password);
    IPAddress IP = WiFi.softAPIP();
    Serial.printf("Access Point started. IP address: %s\n", IP.toString().c_str());

    setupRoutes();
    server.begin();
    Serial.println("HTTP server started.");
}

void AccessPoint::stop()
{
    server.stop();
    WiFi.softAPdisconnect(true);
    Serial.println("Access Point stopped.");
}

void AccessPoint::handleRequests()
{
    server.handleClient();
}

void AccessPoint::run()
{
    start(); // Uruchom Access Point

    bool credentialsSaved = false; // Flaga wskazująca, czy zapisano dane
    while (!credentialsSaved)
    {
        handleRequests();

        // Sprawdzanie zapisanych danych w pamięci po interakcji użytkownika
        Preferences preferences;
        preferences.begin("wifi", true); // Tryb odczytu
        String ssid = preferences.getString("ssid", "");
        String password = preferences.getString("password", "");
        String mqttBroker = preferences.getString("mqtt_broker", "");
        bool isConfigured = preferences.getBool("configured", false); // Flaga konfiguracji
        preferences.end();

        if (isConfigured && !ssid.isEmpty() && !password.isEmpty() && !mqttBroker.isEmpty())
        {
            Serial.printf("[AccessPoint] Credentials saved. SSID=%s, MQTT Broker=%s\n",
                          ssid.c_str(), mqttBroker.c_str());
            credentialsSaved = true; // Ustawiamy flagę na true, aby wyjść z pętli
        }
        else
        {
            Serial.println("[AccessPoint] Waiting for user to input new credentials...");
        }

        vTaskDelay(500 / portTICK_PERIOD_MS); // Minimalne opóźnienie
    }

    stop(); // Wyłączenie Access Point po zapisaniu danych
}

void AccessPoint::setupRoutes()
{
    // Ustawienie tras dla serwera HTTP
    server.on("/", HTTP_GET, [this]()
              { this->handleRoot(); });
    server.on("/save", HTTP_POST, [this]()
              { this->handleSave(); });
}

void AccessPoint::handleRoot()
{
    // Wyświetlenie formularza do wpisania danych WiFi i MQTT
    server.send(200, "text/html",
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
                "<label for='mqtt_client_id'>MQTT Client ID (optional):</label>"
                "<input type='text' id='mqtt_client_id' name='mqtt_client_id'><br>"
                "<button type='submit'>Save</button></form>"
                "</body></html>");
}

void AccessPoint::handleSave()
{
    if (server.hasArg("ssid") && server.hasArg("password") && server.hasArg("mqtt_broker") && server.hasArg("mqtt_port"))
    {
        String ssid = server.arg("ssid");
        String password = server.arg("password");
        String mqttBroker = server.arg("mqtt_broker");
        int mqttPort = server.arg("mqtt_port").toInt();
        String mqttClientId = server.hasArg("mqtt_client_id") ? server.arg("mqtt_client_id") : "ESP32Client";

        // Zapis danych Wi-Fi i MQTT w pamięci trwałej
        Preferences preferences;
        preferences.begin("wifi", false); // Tryb zapisu
        preferences.putString("ssid", ssid);
        preferences.putString("password", password);
        preferences.putString("mqtt_broker", mqttBroker);
        preferences.putInt("mqtt_port", mqttPort);
        preferences.putString("mqtt_client_id", mqttClientId);
        preferences.putBool("configured", true); // Flaga konfiguracji
        preferences.end();

        Serial.printf("Saved settings: SSID=%s, PASSWORD=%s, MQTT Broker=%s, Port=%d, Client ID=%s\n",
                      ssid.c_str(), password.c_str(), mqttBroker.c_str(), mqttPort, mqttClientId.c_str());

        server.send(200, "text/html",
                    "<html><body><h1>Settings Saved. AP will stop now.</h1></body></html>");

        stop(); // Wyłączenie Access Point
    }
    else
    {
        server.send(400, "text/html",
                    "<html><body><h1>Missing parameters</h1></body></html>");
    }
}

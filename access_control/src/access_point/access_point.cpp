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
        bool isConfigured = preferences.getBool("configured", false); // Flaga konfiguracji
        preferences.end();

        if (isConfigured && !ssid.isEmpty() && !password.isEmpty())
        {
            Serial.printf("[AccessPoint] WiFi credentials saved. SSID=%s\n", ssid.c_str());
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
    // Use lambdas to bind non-static member functions
    server.on("/", HTTP_GET, [this]()
              { this->handleRoot(); });
    server.on("/save", HTTP_POST, [this]()
              { this->handleSave(); });
}

void AccessPoint::handleRoot()
{
    server.send(200, "text/html",
                "<html><body><h1>ESP Access Point</h1>"
                "<form method='POST' action='/save'>"
                "<label for='ssid'>WiFi SSID:</label>"
                "<input type='text' id='ssid' name='ssid'><br>"
                "<label for='password'>WiFi Password:</label>"
                "<input type='password' id='password' name='password'><br>"
                "<label for='mqtt'>MQTT Broker:</label>"
                "<input type='text' id='mqtt' name='mqtt'><br>"
                "<button type='submit'>Save</button></form>"
                "</body></html>");
}

void AccessPoint::handleSave()
{
    if (server.hasArg("ssid") && server.hasArg("password"))
    {
        String ssid = server.arg("ssid");
        String password = server.arg("password");

        // Zapis danych Wi-Fi w pamięci trwałej
        Preferences preferences;
        preferences.begin("wifi", false); // Tryb zapisu
        preferences.putString("ssid", ssid);
        preferences.putString("password", password);
        preferences.putBool("configured", true); // Flaga konfiguracji
        preferences.end();

        Serial.printf("Saved settings: SSID=%s, PASSWORD=%s\n", ssid.c_str(), password.c_str());

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
